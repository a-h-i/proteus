#pragma once
#include "../common/exceptions.hpp"
#include <list>
#include <string>
#include <unordered_map>


namespace ui {
std::list<std::string> convertArgs( std::list<std::string> &, const int argc,
                                    const char *const *const argv  );


enum class OptionKey {
    EXECUTABLE_DIRECTORY, CONFIGURATION_FILES, TEMPLATE_FILES, CONF_WARNINGS
};
namespace std {
template<>
struct hash<OptionKey> {
    size_t operator()( const OptionKey &e ) const {
        return std::hash<std::underlying_type<OptionKey>::type>()(
                   static_cast<std::underlying_type<OptionKey>::type>( e ) );
    }
};
}

/*************************************************************************
 * Iterates over an argument list, to create key-value pairs of options. *
 * Consumes the ones it understood.                                      *
 * Throws NoWorkException if the program should not do any work. i.e -h  *
 * opt                                                                   *
 *************************************************************************/
std::unordered_multimap<OptionKey, std::string> getConfiguration(
    std::list<std::string> & );
}