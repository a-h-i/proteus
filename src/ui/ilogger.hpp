#pragma once
#include <string>
#include <type_traits>

/**************************
 *Basic Logging Interface *
 **************************/
struct ILogger {
    virtual ILogger &operator<<( const std::string & ) = 0;
    virtual ILogger &operator<<( const char * ) = 0;
    virtual ILogger &operator<<( const char ) = 0;
    template<typename T>
    typename std::enable_if<std::is_integral<T>::value, ILogger>::type
      &operator<< ( T &&stringfiable ) {
        return ( *this ) << std::to_string( stringfiable );
    }
    virtual ~ILogger(){}
};