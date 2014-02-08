#pragma once
#include "symtable.hpp"
#include "typedefs.hpp"
#include "../common/exceptions.hpp"
#include "../ui/ilogger.hpp"
#include <memory>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

namespace gen {
namespace parsing {




class ConfigurationParser {
    logger_ptr_t logger;
    std::unordered_map<sentence_ptr_t, id_t, sentValueHash, sentValueEq> map_;
    std::unordered_set<sentence_ptr_t, sentValueHash, sentValueEq> sents_;
    SymTable syms;
    bool error_;
public:
    typedef char warningLevel_t;
    static const char Wall;
    static const char WDuplicate; // duplicate sentence found and will be ignored
    static const char WUnused;// unused variables
    static const char WReAssign; // variable re assigned
    static const char DEFAULT_WARN;


    /***************************************************
     * Invokes parseFile and thus can throw exceptions *
     ***************************************************/
    ConfigurationParser( const boost::filesystem::path &file,
                         logger_ptr_t logger, const warningLevel_t warnings = DEFAULT_WARN );

    ConfigurationParser( logger_ptr_t logger,
                         const warningLevel_t warnings = DEFAULT );
    /**********************************************************************
     * Parses a configuration file. A configuration can be parsed         *
     * from more than one configuration file, duplicate variables will be *
     * overwriten and duplicate sentences will be ignored.                *
     * Throws FileError if File is not a regular file                     *
     **********************************************************************/
    void parseFile( const boost::filesystem::path &file );
    auto sentences() -> decltype( sents_ ) {
        return sents_;
    }
    auto  map() -> decltype( map_ ) {
        return map_;
    }
    bool error() const {
        return error_;
    }

    auto sentences() const -> const decltype(  sents_ ) & {
        return sents_;
    }
    auto  map() const -> const decltype(  map_ ) & {
        return map_;
    }
    sent_map_t getFinalMap() {
        sent_map_t finalForm;
        for(auto pair : map_ ) {
            finalForm[*pair.first] = pair.second;
        }
        return finalForm;
    }

private:
    warningLevel_t warnLevel;
    /******************************************
     * Resolves dependencies of a variable.   *
     * Used in the case of forward declaring. *
     * ****************************************/
    void fillDependecies( const std::string & );
    /**************************************
     * Helper function for parsing a line *
     * defining a variable.               *
     **************************************/
    void handleVarDef( boost::smatch &, const boost::filesystem::path &,
                       const std::size_t );
    /*******************************
     * Helper function for parsing *
     * a line defining a sentence. *
     *******************************/
    void handleSentence( boost::smatch &, const boost::filesystem::path &,
                         const std::size_t );
};



template <typename Stream>
void print( Stream &&s, const ConfigurationParser &conf ) {
    s << "Error: " <<  ( conf.error() ? "True" : "False" )
      << "\nSentences:\n";

    for ( auto &sent : conf.sentences() ) {
        s << *sent << '\n';
    }


    s << "(Sentence , ID) pairs:\n";

    for (  auto &siPair : conf.map() ) {
        s << "( " <<  *siPair.first  << " , "
          << siPair.second << " )\n";
    }
}

}
}