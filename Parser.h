/*
 * File:   Parser.h
 *
 * Created on 24 sierpnia 2019, 14:24
 */

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "Utils.h"

class Parser
{
public:
    Parser();
    virtual ~Parser();

    int parseDir(const std::string &path);
    std::vector<std::string> getFoundTexts();

private:
    void parseFile(const std::string &file);
    const std::string getFileExt(const std::string &file);
    const std::string getFileContent(const std::string &file);
    const std::string findTextBeetwenQuotations(
        const std::string &content,
        std::size_t start,
        std::size_t &out_end,
        std::string &out_quote_mark
    );

    std::vector<std::string> results;
    Utils* pUtils;
};

#endif /* PARSER_H */
