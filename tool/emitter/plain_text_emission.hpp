#pragma once
#include "emission.hpp"
#include <string>

namespace emitter {

/*********************
 * Plain and Simple. *
 *********************/
class PlainTextEmission : public Emission {
    const std::string text;
public:
    PlainTextEmission( const std::shared_ptr<bfs::ofstream>  &file,
                       const std::string &text ) : Emission( file ), text( text ) {}
    void emit() override;

};
}