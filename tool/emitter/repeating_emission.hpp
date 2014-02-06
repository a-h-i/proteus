#pragma once
#include "emission.hpp"
#include <boost/regex.hpp>

namespace emitter {

/***********************************
 * Repeats a block to.             *
 * represent a range in a context. *
 ***********************************/
class RepeatingEmission : public Emission {
    const std::string mold;
    const boost::regex pattern;
    std::list<std::shared_ptr<std::string>> range;
public:
    RepeatingEmission( const std::shared_ptr<bfs::ofstream>  &file,
                       const std::string &mold, const boost::regex &pattern,
                       const std::list<std::shared_ptr<std::string>> &range ) : Emission( file ),
        mold( mold ), pattern( pattern ), range( range ) {}
        void emit() override;
};
}