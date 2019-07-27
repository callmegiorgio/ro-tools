#ifndef RO_INVALIDFILE_HPP
#define RO_INVALIDFILE_HPP

#include <exception>
#include <string>

class InvalidFile : public std::exception {
public:
    explicit InvalidFile(const char* what_arg) : c_what_{ what_arg } {}
    
    explicit InvalidFile(const std::string& what_arg)
        : what_{ what_arg }
        , c_what_{ what_.c_str() } {}

    const char* what() const noexcept override { return c_what_; }

private:
    std::string what_;
    const char* c_what_;
};

#endif // RO_INVALIDFILE_HPP