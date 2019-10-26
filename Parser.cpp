
/*
 * File:   Parser.cpp
 *
 * Created on 24 sierpnia 2019, 14:24
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include "Parser.h"

const char* START_UNDERSCORE_FUNC = "__(";
const char* START_LANG_FUNC = "@lang(";

/**
 * Constructor
 */
Parser::Parser()
{
    pUtils = new Utils();
}

/**
 * Destructor
 */
Parser::~Parser()
{
    delete pUtils;
}

/**
 * Open given directory and scan the files with sub directories
 *
 * @param path
 * @return 1 if error, otherwise 0
 */
int Parser::parseDir(const std::string &path)
{
    DIR* dpdf = opendir(path.c_str());
    if (dpdf == NULL) {
        std::cerr << "ERROR: The path " << path << " not found" << std::endl;
        return 1;
    }

    struct dirent *epdf;
    while (epdf = readdir(dpdf)) {
        const std::string name = epdf->d_name;
        switch (epdf->d_type) {
            case DT_REG: // file
                parseFile(path + "/" + name);
                break;

            case DT_DIR: // directory, dig deeper
                if (name != "." &&
                    name != ".." &&
                    name != "node_modules"
                ) {
                    parseDir(path + "/" + name);
                }
                break;
        }
    }
    closedir(dpdf);

    return 0;
}

/**
 * Parse given file
 *
 * @param file The file to parse
 */
void Parser::parseFile(const std::string &file)
{
    if (getFileExt(file) != "php") {
        // Parse only PHP file
        return;
    }

    // Read whole file to string
    const std::string content = pUtils->getFileContent(file);

    std::size_t conten_length = content.length();
    std::size_t cursor = 0;
    while (cursor < conten_length) {
        // find start marker
        std::size_t start_ignore = content.find("__()", cursor);
        std::size_t start__ = content.find(START_UNDERSCORE_FUNC, cursor);
        std::size_t start_lang = content.find(START_LANG_FUNC, cursor);
        if (start_ignore != std::string::npos && start_ignore == start__) {
            // empty __(), skip it and continue
            cursor += 4;
            continue;
        }

        std::size_t start = std::min(start__, start_lang);
        if (start == std::string::npos) {
            break;
        }

        std::size_t startMarkerLen = (
            start == start__
                ? strlen(START_UNDERSCORE_FUNC)
                : strlen(START_LANG_FUNC)
        );

        // find first text into quotation mark
        std::size_t end = std::string::npos;
        std::string quote_mark;
        const std::string text = findTextBeetwenQuotations(content, start + startMarkerLen, end, quote_mark);
        if (text.empty()) {
            cursor = (start + startMarkerLen);
            continue;
        }
        results.push_back(text);

        // Check which char is next after ending quote_mark, whether . whether )
        // If it's . then we have continuation of string
        std::size_t i = end + 1;
        std::size_t cursorShift = i;
        for (; i < conten_length; ++i) {
            if (content[i] == ')' || content[i] == ',') {
                // we can search next __(
                break;
            }

            if (content[i] == '.') {
                // we have concatenation of string, se search start and end of this string
                std::size_t new_end = std::string::npos;
                const std::string text = findTextBeetwenQuotations(content, i, new_end, quote_mark);
                if (!text.empty()) {
                    results[results.size() - 1] += text;
                }
                i = new_end;
                cursorShift = i;
            }
        }

        cursor = cursorShift;
    }
}

/**
 * Get extension of given file
 *
 * @param file The file name
 * @return Extension of file or empty string if not found
 */
const std::string Parser::getFileExt(const std::string &file) {

   std::size_t pos = file.rfind(".", file.length());
   if (pos != std::string::npos) {
      return file.substr(pos + 1, file.length() - pos);
   }

   return "";
}

/**
 * Find text beetwen quotations
 *
 * @param content The file content
 * @param start Start position of cursor
 * @param out_end Output of end of found text
 * @param out_quote_mark Outpurt of found quotation mark. It can be single ' or double " quote.
 * @return
 */
const std::string Parser::findTextBeetwenQuotations(
    const std::string &content,
    std::size_t start,
    std::size_t &out_end,
    std::string &out_quote_mark
) {
    std::size_t start_no_quote = content.find(")", start);
    std::size_t start_quote1 = content.find("'", start);
    std::size_t start_quote2 = content.find("\"", start);
    std::size_t start_quote = std::min(start_quote1, start_quote2);
    if (start_no_quote != std::string::npos && start_no_quote < start_quote) {
        // there is no string into lang function, e.g. we are in regular expression _(?!_)
        return std::string();
    }
    out_quote_mark.assign(1, content[start_quote]);

    // search end string
    out_end = std::string::npos;
    std::size_t tmp = start_quote;
    while (true) {
        out_end = content.find(out_quote_mark, tmp + 1);
        if (out_end == std::string::npos || out_end <= 0) {
            break;
        }

        if (content[out_end - 1] == '\\') {
            // check for \' in string, if it's \' continue searching the end of string
            tmp += 2;
            continue;
        }

        break;
    }

    std::string text = content.substr(start_quote + 1, out_end - start_quote - 1);
    text = pUtils->replaceInText(text, "\\'", "'");
    return text;
}

/**
 * Get array of all found texts
 *
 * @return The array of texts
 */
std::vector<std::string> Parser::getFoundTexts()
{
    // sort & remove duplicates
    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end()), results.end());

    return results;
}
