#include "../grammar.hpp"
#include <unordered_set>
#include <type_traits>
#include <numeric>
#include <iterator>
#include <algorithm>


using namespace gen;
using namespace gen::grammar;
static production_t rightFactor( std::list<production_t> &productions );
static production_t leftFactor( std::list<production_t> &productions );

production_t optimizers::simpleFactoringOptimizer( std::list<production_t>
        &productions ) {
    // we make use of a set so that duplicates
    // are ignored. Which may appear after factoring
    // A set can not be modified in place.
    // choosing a map would result in
    // unnecessary copies as well
    // an unordered set was chosen
    // for hash efficiency
    struct ProductionHash {
        std::size_t operator()( const production_t &p ) const {
            std::string value = std::accumulate( p.begin(), p.end(), std::string() );
            return std::hash<std::string>()( value );
        }
    };
    std::unordered_set<production_t, ProductionHash> optimizedSet;
        // All 1 terminal productions are already optimized
    for ( auto itr = productions.begin(); itr != productions.end(); ++itr ) {
        if ( itr->size() == 1 ) {
            optimizedSet.insert( std::move( *itr ) );
            auto prev = std::prev( itr, 1 );
            productions.erase( itr ); // invalidates itr
            itr = prev;
        }
    }


    // now right factor
    // right factoring has precedence over left factoring
    // i.e if both are possible right is taken.
    // rightFactor and leftFactor subroutines remove
    // entries from productions list if they can be factored
    // otherwise they are preserved. But not necessarily
    // in the same order.
    optimizedSet.emplace( rightFactor( productions ) );
    // now left factor the remaining productions
    optimizedSet.emplace( leftFactor( productions ) );
    // combine optimized and remaining ones
    production_t prod; // final form



    for ( auto &i : optimizedSet ) {
        if ( !i.empty() ) {
            if ( prod.empty() ) {
                prod.emplace_back( "(" );
                // copy
                std::copy( std::begin( i ), std::end( i ), std::back_inserter( prod ) );

                prod.emplace_back( ")" );
            } else {
                prod.emplace_back( "|(" );
                std::copy( std::begin( i ), std::end( i ), std::back_inserter( prod ) );
                prod.emplace_back( ")" );
            }
        }
    }

    for ( auto itr = productions.begin(); itr != productions.end(); ++itr ) {
        auto prev = std::prev( itr, 1 );

        if ( prod.empty() ) {
            prod.emplace_back( "(" );
            prod.splice( prod.end(), *itr );
            prod.emplace_back( ")" );
        } else {
            prod.emplace_back( "|(" );
            prod.splice( prod.end(), *itr );
            prod.emplace_back( ")" );
        }

        productions.erase( itr );
        itr = prev;
    }

    return prod;
}

using namespace gen::optimizers;
static production_t rightFactor( std::list<production_t> &productions ) {
    // (suffix, list of productions with that suffix)
    // list used over a multi_map for convenience
    std::map<std::string, std::list<production_t>> buckets;

    // Populate buckets
    for ( auto itr = productions.begin(); itr != productions.end(); ++itr ) {
        std::string suffix = itr->back();
        itr->pop_back();
        auto prev = std::prev( itr, 1 );
        buckets[suffix].emplace_back( std::move( *itr ) );
        productions.erase( itr );
        itr = prev;
    }

    production_t optimized;

    // merge and factor out
    for ( auto &pair : buckets ) {
        if ( pair.second.size() >
                1 ) { // more than one production with shared suffix
            if ( optimized.empty() ) {
                optimized.emplace_back( "((" );
            } else {
                optimized.emplace_back( "|((" );
            }

            optimized.splice( optimized.end(), simpleFactoringOptimizer( pair.second ) );
            optimized.emplace_back( std::string( ")" ) + pair.first + ")" );
        } else {
            // no possible right factoring
            // return production to list of productions
            productions.emplace_back( pair.second.front() );
            productions.back().emplace_back( pair.first );
        }
    }

    return optimized;
}


static production_t leftFactor( std::list<production_t> &productions ) {
 
    // (prefix, list of productions with that prefix)
    std::map<std::string, std::list<production_t>> buckets;

    // populate buckets
    for ( auto itr = productions.begin(); itr != productions.end(); ++itr ) {
        auto prev = std::prev( itr, 1 );
        std::string prefix = itr->front();
        itr->pop_front();
        buckets[prefix].emplace_back( std::move( *itr ) );
        productions.erase( itr );
        itr = prev;
    }

    production_t optimized;

    //merge and factor out
    for ( auto &pair : buckets ) {
        if ( pair.second.size() > 1 ) { //more than one production with shared prefix
            if ( optimized.empty() ) {
                optimized.emplace_back( "(" + pair.first + "(" );
            } else {
                optimized.emplace_back( "|(" + pair.first + "(" );
            }

            optimized.splice( optimized.end(), simpleFactoringOptimizer( pair.second ) );
            optimized.emplace_back( "))" );
        } else {
            // no possible left factoring
            // return production to list of productions
            productions.emplace_back( pair.second.front() );
            productions.back().emplace_front( pair.first );
        }
    }

    return optimized;
}