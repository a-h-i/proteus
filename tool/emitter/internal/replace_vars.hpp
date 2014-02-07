#pragma once
#include "replace_vars.hpp"
#include "../../generator/typedefs.hpp"
namespace emitter {
namespace internal {


std::string replaceVariables( const std::string &text, const std::string &grammarFile,
                              const std::string &dictFile, const gen::sent_map_t  * const map );
}
}