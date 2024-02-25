
/*
 * File:   Output.h
 *
 * Created on 24 sierpnia 2019, 13:16
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Utils.h"

class Output
{
public:
    Output(const std::string& outputFile);
    virtual ~Output();

    void write(std::vector<std::string> texts);
private:
    std::streambuf* getStreamBuffer();
    std::vector< std::pair<std::string, std::string> > getCurrentTextsInOutputFile();

    std::ofstream* pOutputStream = NULL;
    std::string outputFile;
    Utils* pUtils;
};

#endif /* OUTPUT_H */
