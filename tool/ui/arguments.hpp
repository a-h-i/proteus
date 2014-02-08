#pragma once
#include "../common/exceptions.hpp"
#include <list>
#include <string>
#include <unordered_map>


namespace ui {
std::list<std::string> convertArgs( const int argc,
                                    const char *const *const argv  );


enum class OptionKey {
    EXECUTABLE_DIRECTORY, CONFIGURATION_FILES, TEMPLATE_FILES, CONF_WARNINGS
};
}
namespace std {
template<>
struct hash<ui::OptionKey> {
    size_t operator()( const ui::OptionKey &e ) const {
        return std::hash<std::underlying_type<ui::OptionKey>::type>()(
                   static_cast<std::underlying_type<ui::OptionKey>::type>( e ) );
    }
};
}

namespace ui {

/*************************************************************************
 * Iterates over an argument list, to create key-value pairs of options. *
 * Consumes the ones it understood.                                      *
 * Throws NoWorkException if the program should not do any work. i.e -h  *
 * opt. Throws InvalidArgs if parsed an unknown argument.                *
 *************************************************************************/
std::unordered_multimap<OptionKey, std::string> getConfiguration(
    std::list<std::string> && );
}