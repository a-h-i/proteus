#pragma once
#include "../common/exceptions.hpp"
#include "../generator/typedefs.hpp"
#include <fstream>
#include <memory>

namespace emitter {
class Emitter {
protected:
    std::shared_ptr<std::ofstream> file;
public:
    Emitter( const std::shared_ptr<std::ofstream>  &file ) : file( file ) {}
    /************************
     * Throws EmitterError *
     * if outstream is bad  *
     ************************/
    virtual void emit() = 0;
    virtual ~Emitter(){};
};
}