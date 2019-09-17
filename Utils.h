/*
 * File:   Utils.h
 *
 * Created on 28 sierpnia 2019, 23:18
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>

class Utils
{
public:
    const std::string getFileContent(const std::string &file);
    const std::string replaceInText(std::string& str, const std::string& from, const std::string& to);
};

#endif /* UTILS_H */
