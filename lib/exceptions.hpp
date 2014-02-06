#pragma once
#include <stdexcept>
#include <string>
namespace proteus {
namespace exceptions {

struct PortAudio : std::logic_error {
PortAudio( const std::string &what ) noexcept: std::logic_error( what ) {}
};
}
}