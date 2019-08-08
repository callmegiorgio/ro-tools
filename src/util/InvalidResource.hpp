#ifndef RO_INVALIDRESOURCE_HPP
#define RO_INVALIDRESOURCE_HPP

#include <exception>
#include <string>

class InvalidResource : public std::exception {
public:
    explicit InvalidResource(const char* what_arg) : c_what_{ what_arg } {}
    
    explicit InvalidResource(const std::string& what_arg)
        : what_{ what_arg }
        , c_what_{ what_.c_str() } {}

    const char* what() const noexcept override { return c_what_; }

private:
    std::string what_;
    const char* c_what_;
};

#endif // RO_INVALIDRESOURCE_HPP