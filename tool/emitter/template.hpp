#pragma once
#include "../generator/typedefs.hpp"
#include "../ui/ilogger.hpp"
#include "emitter.hpp"
#include <string>
namespace emitter {

using gen::sent_map_t;
void createSequence( const std::string &templateFile, const std::string &dict,
                     const std::string &grammar, const sent_map_t *const,
                     logger_ptr_t logger );
}