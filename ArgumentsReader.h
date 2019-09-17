
/*
 * File:   ArgumentsReader.h
 *
 * Created on 24 sierpnia 2019, 13:53
 */

#ifndef ARGUMENTSREADER_H
#define ARGUMENTSREADER_H

#include <map>
#include <string>

const std::string ARG_PATH = "path";
const std::string ARG_OUTPUT_FILE = "output_file";

class ArgumentsReader
{
public:
    ArgumentsReader(int argc, char** argv);

    /**
      * Get path from arguments
      *
      * @return
      */
    const std::string& getPath() { return arguments[ARG_PATH]; }

    /**
     * Get output file of empty string
     *
     * @return
     */
    const std::string& getOutputFile() { return arguments[ARG_OUTPUT_FILE]; }

    void printHelp();
    bool isExitFlag();

private:
    std::map<std::string, std::string> arguments;
    void parseArguments(int argc, char** argv);
    void printVersion();

    bool isExit;
};

#endif /* ARGUMENTSREADER_H */
