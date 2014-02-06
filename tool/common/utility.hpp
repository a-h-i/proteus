#pragma once
#include <string>
#include <random>
namespace gen {

// true if c in [a, b]
inline bool inRange( int a, int b, int c ) {
    return ( c >= a && c <= b );
}

inline std::string generateRandomString( const std::size_t length ) {
    std::random_device rd;
    std::uniform_int_distribution<int> dist( 'A', 'z' ); // no numbers
    std::string str( length, '-' );

    for ( std::size_t  i = 0; i < length; i++ ) {
        char rc = dist( rd );

        if ( inRange( 91, 96, rc ) ) {
            i--;
            continue;
        }

        str[i] = rc;
    }

    return str;
}
}