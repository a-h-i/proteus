#pragma once
#include "emitter.hpp"
#include <fstream>
namespace emitter {

/***********************************
 * Repeats a block to.             *
 * represent a range in a context. *
 ***********************************/
class RepeatingEmitter : public Emitter {
    const std::string mold;
    const std::string grammarFile;
    const std::string dictFile;
    gen::sent_map_t const *const map;
public:
    RepeatingEmitter( const std::shared_ptr<std::ofstream>  &file,
                      std::string &&mold, const std::string &grammarFile,
                       const std::string &dictFile,
                      gen::sent_map_t const *const map ) : Emitter( file ),
        mold( mold ), grammarFile( grammarFile ), dictFile( dictFile ), map( map ) {}

    void emit() override;
};
}