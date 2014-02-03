#pragma once
#include <stdexcept>



class FileError : public std::logic_error {
public:
FileError( const std::string &str ) noexcept: std::logic_error( str ) {}
};