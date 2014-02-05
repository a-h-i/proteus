#pragma once
#include "parser.hpp"
#include "typedefs.hpp"
#include "../ui/ilogger.hpp"

#include <list>
#include <string>
#include <sstream>
#include <iterator>
#include <functional>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
namespace gen {
namespace grammar {


/*************************************************
 * Rationale:                                    *
 * If we consider the set of possible commands   *
 * as language L. Then since the configuration   *
 * file makes no use of recursion or repetition. *
 * Then L is finite. Thus we can define a CFG    *
 * G that generates L as a grammar with one      *
 * non-terminal , start symbol, and has N        *
 * productions ,where N is the |L|.              *
 * We represent a productions as a doubly-       *
 * ended list of words to ease concatenation     *
 * and transformations of strings.               *
 *                                               *
 *************************************************/



typedef std::list<std::string> production_t;

class Grammar {
    std::list<production_t> productions_;
public:
    typedef std::function<production_t ( decltype( productions_ ) & )> optimizer_t;
    template <class Container>
    Grammar( const Container &sentences ) {
        for ( const auto &s : sentences ) {
            productions_.emplace_back(); // create default constructed production at end
            boost::split( productions_.back(), *s, boost::is_any_of( " " ) );
        }
    }
    void optimize( optimizer_t o ) {
        productions_.push_back( o( productions_ ) );

    }
    const std::list<production_t> &productions() const {
        return productions_;
    }
};


/********************************
 * This function is for logging *
 * and debugging purposes only. *
 ********************************/

template<class Stream>
void print( Stream &&s, const Grammar &g ) {

    const std::list<production_t> &prods = g.productions();
    s << "S --> ";
    std::size_t i = 1;

    for ( const auto &p : prods ) {
        for ( const auto &str : p ) {
            s << str << ' ';
        }

        if ( i == prods.size() ) {
            s << "\n";
        } else {
            s << "| ";
        }

        i++;
    }

    std::cout << "Grammar Size: "  << prods.size() << '\n';
}


std::string createJSGF( const Grammar &g, const std::string &name );

}
namespace optimizers {

/*****
 * Consumes parameter productions.
 * returns the optimized production.
 * Performs simple right and left factoring.
 */
grammar::production_t simpleFactoringOptimizer( std::list<grammar::production_t>
        &productions );
}
}