#pragma once
#include "../generator/typedefs.hpp"
#include "../ui/ilogger.hpp"
#include "emission.hpp"
#include <list>
#include <memory>
#include <unordered_map>
#include <boost/filesystem.hpp>
namespace emitter {

using gen::sent_map_t;
std::list<std::unique_ptr<Emission>> createSequence( const bfs::path
                                  &templateFile, const bfs::path &dict,
                                  const bfs::path &grammar,const sent_map_t *const, 
                                  logger_ptr_t logger );
}