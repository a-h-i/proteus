#pragma once
#include "ilogger.hpp"
#include <iostream>


/*************************************************
 * Basic Logger , logs to standard error stream. *
 * Used for testing Purposes.                    *
 *************************************************/
struct BasicConsoleLogger: Ilogger {

    BasicConsoleLogger &operator<<( const std::string &str ) override {
        std::cerr << str;
        return *this;
    }
    virtual BasicConsoleLogger &operator<<( const char *st ) override {
        std::cerr << str;
        return *this;
    }
    virtual BasicConsoleLogger &operator<<( const char c) override {
        std::cerr << c;
        return *this;
    }
};