/*
 * File:   ArgumentsReader.cpp
 *
 * Created on 24 sierpnia 2019, 13:53
 */

#include <iostream>
#include <getopt.h>
#include <stdio.h>
#include "ArgumentsReader.h"

/**
 * Constructor
 *
 * @param argc Number of arguments
 * @param argv Arguments texts
 */
ArgumentsReader::ArgumentsReader(int argc, char** argv)
{
    isExit = false;
    parseArguments(argc, argv);
}

/**
 * Parse arguments given by executing the program
 *
 * @param argc
 * @param argv
 */
void ArgumentsReader::parseArguments(int argc, char** argv)
{
    static struct option long_options[] = {
        {"output",  required_argument, 0,  'o' },
        {"help",    no_argument,       0,  'h' },
        {"version", no_argument,       0,  'v' },
        {0,         0,                 0,  0 }
    };

    int option;
    while ((option = getopt_long(argc, argv, "o:hv", long_options, NULL)) != -1) {
        switch (option) {
            case 'v':
                printVersion();
                isExit = true;
                break;

            case 'o':
                arguments[ARG_OUTPUT_FILE] = optarg;
                break;

            case 'h':
            case '?':
            default:
                printHelp();
                isExit = true;
                break;
        }
    }

    // optind is for the extra arguments which are not parsed
    for (; optind < argc; optind++) {
        arguments[ARG_PATH] = argv[optind];
    }
}

/**
 * Print help to cout
 */
void ArgumentsReader::printHelp()
{
    std::cout << "Usage: laravel-search-lang <path> [-o <output json file>] [-h] [-v]" << std::endl;
    std::cout << "    <path> - the path to Laravel main directory" << std::endl;
    std::cout << "    -o <file name>, --output <file name> - the jeson output file e.g. pl.json" << std::endl;
    std::cout << "    -h, --help - show this help message and exit" << std::endl;
    std::cout << "    -v, --v - show version message and exit" << std::endl;
}

/**
 * Print version message to cout
 */
void ArgumentsReader::printVersion()
{
    std::cout << "Laravel Search Lang version 1.0" << std::endl;
}

/**
 * Check whether app should be exit
 *
 * @return
 */
bool ArgumentsReader::isExitFlag()
{
    return isExit;
}
