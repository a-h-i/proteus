#pragma once
#include "emitter.hpp"
#include <string>

namespace emitter {

/*********************
 * Plain and Simple. *
 *********************/
class PlainTextEmitter : public Emitter {
    const std::string text;
public:
    PlainTextEmitter( const std::shared_ptr<std::ofstream>  &file,
                        std::string &&text ) : Emitter( file ), text( text ) {}
    PlainTextEmitter( const std::shared_ptr<std::ofstream>  &file,
                        std::string &text ) : Emitter( file ), text( text ) {}
    void emit() override;

};
}