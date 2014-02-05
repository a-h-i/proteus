#include "grammar.hpp"
#include <set>
#include <iterator>

using namespace gen;
using namespace gen::grammar;
production_t optimizers::simpleFactoringOptimizer(std::list<production_t> &productions){
    std::set<production_t> optimizedSet;

    // All 1 terminal productions are already optimized
    for(auto itr = productions.begin(); itr != productions.end(); ++itr){
        if(itr->size() == 1) {
            optimizedSet.insert(std::move(*itr));
            auto prev = std::prev(itr, 1);            
            productions.erase(itr); // invalidates itr
            itr = prev;
        }
    }

    // now right factor


    // now left factor

    // combine 
    
    return {};
}