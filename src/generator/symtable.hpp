#pragma once
#include "typedefs.hpp"
#include <set>
#include <list>
#include <memory>
#include <type_traits>
#include <unordered_map>


namespace gen {

/***********************************
 * Represents a SymbolTable Entry. *
 * Also keeps track of sentences   *
 * that depend on a vartiable that *
 * was defined after use.          *
 ***********************************/
struct Entry {
    std::string identifier;
    std::string value;
    std::string fileName;
    std::size_t lineNumber;
    bool used;
    Entry() = default;
    Entry( const Entry & ) = default;
    Entry( Entry && ) = default;
    Entry &operator=( const Entry & ) = default;
    Entry &operator=( Entry && ) = default;
};

class SymTable {
    std::unordered_map<std::string, Entry> table;
    std::unordered_map<std::string, std::shared_ptr<std::list<sentence_t>> > deps;
public:

    /*************************************************
     * Checks if a definition for a variable exists. *
     *************************************************/
    bool contains( const std::string &identifier ) {
        return table.count( identifier ) != 0;
    }

    /**********************************************************
     * Defines a new variable, or replaces an old definition. *
     **********************************************************/
    void addDeff( const std::string &identifier, const std::string &value,
                  const std::string &fileName, const std::size_t lineNumber ) {
        table[identifier] = {identifier, value, fileName, lineNumber, false};
    }

    /************************************************
     * Erases dependency List of a single variable. *
     ************************************************/
    void removeDeps( const std::string &identifier ) {
        deps.erase( identifier );
    }

    void addDep( sentence_t &sent, const std::string &identifier ) {
        if ( deps.count( identifier ) == 0 ) {
            deps.emplace( identifier, std::make_shared<std::list<sentence_t>>() );
        }

        deps[identifier]->push_back( sent );
    }

    /**********************************************************
     * Retrives list of sentences that used identifier before *
     * it was defined. Creates one if it doesn't exist.       *
     **********************************************************/
    auto dependecies( const std::string &identifier,
                      bool remove = true ) -> typename
    std::remove_reference<decltype( deps[0] )>::type {
        if ( deps.count( identifier ) == 0 ) {
            deps.emplace( identifier, std::make_shared<std::list<sentence_t>>() );
        }

        auto list = deps[identifier];

        if ( remove ) {
            removeDeps( identifier );
        }

        return list;
    }

    void unDefine( const std::string &identifier ) {
        table.erase( identifier );
    }

    Entry &operator[]( const std::string &identifier ) {
        return table[identifier];
    }
    std::set<std::string> undefinedVariables() const {
        std::set<std::string> uvars;

        for ( const auto &vlPair : deps ) {
            if ( !vlPair.second->empty() ) {
                uvars.insert( vlPair.first );
            }
        }

        return uvars;
    }

    std::set<std::string> unUsedVariables() const {
        std::set<std::string> unused;

        for ( const auto &vePair : table ) {
            if ( !vePair.second.used ) {
                unused.insert( vePair.first );
            }
        }

        return unused;
    }
};
}