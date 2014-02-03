#pragma once
#include "../ui/ilogger.hpp"
#include "exceptions.hpp"
#include "symtable.hpp"
#include "typedefs.hpp"
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
    std::shared_ptr<ILogger> logger;
    std::unordered_map<sentence_t, id_t, sentValueHash> map_;
    std::unordered_set<sentence_t, sentValueHash> sents_;
    SymTable syms;
    bool error_;
public:
    typedef char warningLevel_t;
    static constexpr char Wall = 0xFF;
    static constexpr char WDuplicate =
        0x01; // duplicate sentence found and will be ignored
    static constexpr char WUnused = 0x02; // unused variables
    static constexpr char WReAssign = 0x04; // variable re assigned

    /***************************************************
     * Invokes parseFile and thus can throw exceptions *
     ***************************************************/
    ConfigurationParser( const boost::filesystem::path &file,
                         std::shared_ptr<ILogger> logger, const warningLevel_t warnings = Wall );

    ConfigurationParser( std::shared_ptr<ILogger> logger,
                         const warningLevel_t warnings );
    /**********************************************************************
     * Parses a configuration file. A configuration can be parsed         *
     * from more than one configuration file, duplicate variables will be *
     * overwriten and duplicate sentences will be ignored.                *
     * Throws FileError if File is not a regular file                     *
     **********************************************************************/
    void parseFile( const boost::filesystem::path &file );
    auto sentences() const -> decltype(sents_) {
        return sents_;
    }
    auto  map() const -> decltype(map_) {
        return map_;
    }
    bool error() const {
        return error_;
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

}
}