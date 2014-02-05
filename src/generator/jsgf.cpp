#include "grammar.hpp"
using namespace gen::grammar;

std::string printProduction(const production_t &p) {
    std::string str;
    for ( auto itr = p.begin() ; itr != p.end(); itr++ ) {
            //boost::trim(*itr);
            str += *itr;
            auto next = std::next( itr, 1 );
            if (  next != p.end() ) {
                str += " ";
            }
        }
    return str;
}
std::string gen::grammar::createJSGF( const Grammar &g, const std::string &name ){
    std::ostringstream buff;
    buff << "#JSGF V1.0;\ngrammar "
        << name << ";\n";
    buff << "public <start> = ";
    auto &prods = g.productions();
    bool shouldOr = false;

    for ( const auto &p : prods ) {
        if ( !shouldOr ) {
            buff << "(";
            shouldOr = true;
        } else {
            buff << "|(";
        }
        buff << printProduction(p);
        buff << ")";
    }

    std::string str = buff.str();
    auto space = []( char lhs, char rhs ) {
        return ( lhs == rhs ) & ( rhs == ' ' );
    };
    // collapse spaces
    auto new_end = std::unique( str.begin(), str.end(), space );
    str.erase( new_end , str.end());

    auto pred = boost::is_any_of("()|");


    for(auto i = str.begin(); i != str.end(); i++) {
        if(*i == ' '){
            if(i != str.begin() && pred(*(i-1)) ) {
                str.erase(i);
            }else if((i +1 ) != str.end() && pred(*(i+1))) {
                str.erase(i);
            }
        }
    }

    return str;
}