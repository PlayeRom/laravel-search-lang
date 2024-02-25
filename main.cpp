#include <stdlib.h>
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
 * @return int
 */
int main(int argc, char** argv)
{
    ArgumentsReader argsReader(argc, argv);
    if (argsReader.isExitFlag()) {
        return EXIT_FAILURE;
    }

    const std::string path = argsReader.getPath();

    if (path.empty()) {
        std::cerr << "ERROR: you must set the path to Laravel project as an argument" << std::endl;
        argsReader.printHelp();
        return EXIT_FAILURE;
    }

    Parser parser;
    if (parser.parseDir(path)) {
        return EXIT_FAILURE;
    }

    Output output(argsReader.getOutputFile());
    output.write(parser.getFoundTexts());

    return EXIT_SUCCESS;
}
