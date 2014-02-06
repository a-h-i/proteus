#pragma once
#include "../common/exceptions.hpp"
#include "../generator/typedefs.hpp"
#include <boost/filesystem/fstream.hpp>
#include <memory>

namespace emitter {
namespace bfs = boost::filesystem;
class Emission {
protected:
    std::shared_ptr<bfs::ofstream> file;
public:
    Emission( const std::shared_ptr<bfs::ofstream>  &file ) : file( file ) {}
    /************************
     * Throws EmissionError *
     * if outstream is bad  *
     ************************/
    virtual void emit() = 0;
    virtual ~Emission(){};
};
}