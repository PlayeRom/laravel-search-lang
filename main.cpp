#include <iostream>
#include <string>

#include "Output.h"
#include "ArgumentsReader.h"
#include "Parser.h"

/**
 * Entry point function
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv)
{
    ArgumentsReader argsReader(argc, argv);
    if (argsReader.isExitFlag()) {
        return 1;
    }

    const std::string path = argsReader.getPath();

    if (path.empty()) {
        std::cerr << "ERROR: you must set the path to Laravel project as an argument" << std::endl;
        argsReader.printHelp();
        return 1;
    }

    Parser parser;
    if (parser.parseDir(path)) {
        return 1;
    }

    Output output(argsReader.getOutputFile());
    output.write(parser.getFoundTexts());

    return 0;
}
