#ifndef RO_FILEREADER_HPP
#define RO_FILEREADER_HPP

#include <cstdint>
#include <exception>
#include <fstream>
#include <string>
#include "Buffer.hpp"

class FileNotOpen : public std::exception
{
public:
    explicit FileNotOpen(const std::string& filename) : what_{ "could not open file '" + filename + "'" } {}

    const char* what() const noexcept override { return what_.c_str(); }

private:
    std::string what_;
};

Buffer readFile(const char* filename)
{
    // Open file and set cursor at the end
    std::ifstream ifs(filename, std::ifstream::binary | std::ifstream::ate);

    if (!ifs)
        throw FileNotOpen(filename);

    const size_t size = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    uint8_t* buf = new uint8_t[size];
    ifs.read(reinterpret_cast<char*>(buf), size);

    return Buffer(buf, size);
}

static_assert(sizeof(char) == sizeof(uint8_t), "Error: char size is not 1 byte");

#endif // RO_FILEREADER_HPP