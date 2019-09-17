
/*
 * File:   Utils.cpp
 *
 * Created on 28 sierpnia 2019, 23:18
 */

#include "Utils.h"

/**
 * Get file content as a string
 *
 * @param file The file name
 * @return The file content or empty string if failed
 */
const std::string Utils::getFileContent(const std::string &file)
{
    /*
    std::ifstream input(file.c_str());
    if (!input) {
        return "";
    }
    std::string content(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    input.close();

    return content;*/

    FILE* pfile = fopen(file.c_str(), "rb");
    if (!pfile) {
        return "";
    }

    // in C++17 following lines can be folded into std::filesystem::file_size invocation
    if (fseek(pfile, 0, SEEK_END) < 0) {
      return "";
    }

    const long size = ftell(pfile);
    if (size < 0) {
      return "";
    }

    if (fseek(pfile, 0, SEEK_SET) < 0) {
        return "";
    }

    std::string content;
    content.resize(size);

    std::size_t num = fread(const_cast<char*>(content.data()), 1, size, pfile);
    fflush(pfile);
    fclose(pfile);

    if (num != size) {
        return "";
    }

    return content;
}

/**
 * Replace in text
 *
 * @param text The full text to searc in
 * @param from The phrase to replace
 * @param to The phrase which will be set
 * @return The replaced full text
 */
const std::string Utils::replaceInText(std::string& text, const std::string& from, const std::string& to)
{
    if (from.empty()) {
        return text;
    }
    size_t start_pos = 0;
    while ((start_pos = text.find(from, start_pos)) != std::string::npos) {
        text.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }

    return text;
}
