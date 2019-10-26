/*
 * File:   Output.cpp
 *
 * Created on 24 sierpnia 2019, 13:16
 */

#include <string.h>
#include <vector>
#include <algorithm>
#include "Output.h"

/**
 * Constructor
 *
 * @param outputFile
 */
Output::Output(const std::string& outputFile)
{
    pOutputStream = new std::ofstream;
    this->outputFile = outputFile;
    pUtils = new Utils();

}

/**
 * Destructor
 */
Output::~Output()
{
    delete pOutputStream;
    delete pUtils;
}

/**
 * Write result to output as a file or console
 *
 * @param texts Strings array with found texts
 */
void Output::write(std::vector<std::string> texts)
{
    // if the output file already exist merge the results
    std::vector< std::pair<std::string, std::string> > currentTexts = getCuttentTextsInOutpurFile();

    std::ostream out(getStreamBuffer());

    // Output results to console or to the file
    std::size_t size = currentTexts.size();

    out << "{" << std::endl;

    // Write currentTexts first and remove duplicate from texts vector
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::size_t i = 0;
    bool is_written = false;
    for (it = currentTexts.begin(); it != currentTexts.end(); it++) {
        // out values from keys
        if (i > 0 && i < size) {
            out << "," << std::endl;
        }
        out << "    \"" << it->first << "\": " << "\"" << it->second << "\"";

        // For some reason std::find has problem with "\\'" like "We\\'ll"
        std::vector<std::string>::iterator vit = std::find(texts.begin(), texts.end(), it->first);
        if (vit != texts.end()) {
            // found text in vector, remove it for avoid duplication
            texts.erase(vit);
        }

        ++i;
        is_written = true;
    }

    // Write the remaining texts on the output
    size = texts.size();
    for (i = 0; i < size; ++i) {
        if ((i > 0 || is_written) && i < size) {
            out << "," << std::endl;
        }
        std::string text = texts[i];
        out << "    \"" << text << "\": " << "\"" << text << "\"";
    }
    out << std::endl << "}" << std::endl;
    out.flush();

    // Close file output stream if opened
    if (pOutputStream->is_open()) {
        pOutputStream->close();
    }
}

/**
 * Get buffer to stream as a file or console
 *
 * @param outpuFileName
 * @return streambuf from ofstream or cout
 */
std::streambuf* Output::getStreamBuffer()
{
    if (!outputFile.empty()) {
        pOutputStream->open(outputFile.c_str());
        return this->pOutputStream->rdbuf();
    }

    return std::cout.rdbuf(); // standard cout buffer
}

/**
 * Get current content of output file
 *
 * @return A vector with pairs of current content of file text -> translation
 */
std::vector< std::pair<std::string, std::string> > Output::getCuttentTextsInOutpurFile()
{
    std::vector< std::pair<std::string, std::string> > currentTexts;
    if (!outputFile.empty()) {
        const std::string currentJson = pUtils->getFileContent(outputFile);

        std::string pairValues[2];

        std::size_t cursor = 0;
        int counter = 0;
        while (true) {
            std::size_t start = currentJson.find("\"", cursor);
            std::size_t end = currentJson.find("\"", start + 1);
            while (end != std::string::npos && currentJson[end - 1] == '\\') {
                // we have \" so continue search of end
                end = currentJson.find("\"", end + 1);
            }

            if (start != std::string::npos &&
                end != std::string::npos &&
                start < end
            ) {
                pairValues[counter] = currentJson.substr(start + 1, end - start - 1);

                if (++counter == 2) {
                    std::pair<std::string, std::string> pair(pairValues[0], pairValues[1]);
                    currentTexts.push_back(pair);

                    counter = 0;
                }

                cursor = end + 1;

                continue;
            }

            break;
        }
    }

    return currentTexts;
}
