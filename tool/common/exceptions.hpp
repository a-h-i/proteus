#pragma once
#include <stdexcept>
#include <string>
namespace proteus {
namespace exceptions {

class FileError : public std::logic_error {
public:
FileError( const std::string &str ) noexcept: std::logic_error( str ) {}
};

class InvalidArgs : public std::logic_error {
public:
InvalidArgs( const std::string &str ) noexcept: std::logic_error( str ) {}
};

class EmissionError : public std::logic_error {
public:
    EmissionError(const std::string& str) noexcept: std::logic_error(str) {}
};
}
}