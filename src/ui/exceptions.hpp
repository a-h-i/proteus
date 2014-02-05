#pragma once
#include <string>
#include <stdexcept>
class InvalidArgs : public std::logic_error {
public:
    InvalidArgs(const std::string& str) noexcept: std::logic_error(str) {}
};