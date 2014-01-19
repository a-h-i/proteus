#pragma once
#include <stdexcept>
#include <string>
namespace proteus {
namespace exceptions {

struct Exception : std::runtime_error {
    Exception( const std::string &what ) : std::runtime_error( what ) {}
    Exception( const Exception & ) = default;
    Exception( Exception && ) = default;
    Exception &operator=( const Exception & ) = default;
    Exception &operator=( Exception && ) = default;
};

struct PortAudio : Exception {
    PortAudio( const std::string &what ) : Exception( what ) {}
    PortAudio( const PortAudio & ) = default;
    PortAudio( PortAudio && ) = default;
    PortAudio &operator=( const PortAudio & ) = default;
    PortAudio &operator=( PortAudio && ) = default;
};

}
}