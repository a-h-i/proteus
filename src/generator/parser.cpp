#include "parser.hpp"
#include <set>
#include <iterator>
#include <boost/ref.hpp>
#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/fstream.hpp>

static const boost::regex VAR_DEF (
    R"(([a-z]|[0-9]|[A-Z])+)\s*=\s*(([a-z]|[A-Z])+)" );
static const boost::regex SENTENCE (
    R"(<s>\s*([a-z](\s)?|[A-Z](\s)?)+\s*</s>\s*([a-z]|[A-Z]|[0-9])+)" );
static const boost::regex VAR_REF ( R"($\((([a-z]|[0-9]|[A-Z])+)\))" );

using namespace gen::parsing;

ConfigurationParser::ConfigurationParser( std::shared_ptr<ILogger> logger,
        const warningLevel_t warnings ) :
    logger( logger ),  error_( false ), warnLevel( warnings ) {}

ConfigurationParser::ConfigurationParser( const boost::filesystem::path &file,
        std::shared_ptr<ILogger> logger ,
        const warningLevel_t warnings ) : ConfigurationParser( logger, warnings ) {
    parseFile( file );
}

void ConfigurationParser::parseFile( const boost::filesystem::path &file ) {
    if ( boost::filesystem::is_regular_file( file ) ) {
        // start parsing
        boost::filesystem::ifstream input( file );
        std::string line;
        boost::smatch result;

        for ( std::size_t lineNumber = 1; std::getline( input, line ); lineNumber++ ) {
            boost::trim( line );

            if ( boost::regex_match( line, result, VAR_DEF ) ) {
                handleVarDef( result, file, lineNumber );
            } else if ( boost::regex_match( line, result, SENTENCE ) ) {
                handleSentence( result, file, lineNumber );
            } else {
                *logger  << "Syntax Error: " << line << " " << file.generic_string() << ": " <<
                         lineNumber
                         << '\n';
                error_ = true;
            }
        }

        // check for undefined variables and other warnings / errors
    } else {
        throw FileError( "Path: " + file.generic_string() + " is not a regular file" );
    }
}

void ConfigurationParser::handleVarDef( boost::smatch &result,
                                        const boost::filesystem::path &file, const std::size_t lineNumber ) {
    bool reAssigned = syms.contains( result[1] );

    if ( reAssigned & warnLevel & WReAssign  ) {
        *logger << "Warning redifinition of variable " << result[1]
                << " Originally defined @" << syms[result[1]].fileName  << ':' <<
                syms[result[1]].lineNumber
                << "\nRedifined @" << file.generic_string() << ':' << lineNumber << '\n';
    }

    syms.addDeff( result[1], result[2], file.generic_string(),
                  lineNumber ); // identifier, value,fileName, line

    if ( !reAssigned ) {
        // if its new it might have been forward declared
        // TODO: check and resolve dependencies
        fillDependecies( result[1] );
    }

}

void ConfigurationParser::fillDependecies( const std::string &identifier ) {
    auto depsPtr = syms.dependecies( identifier ); // removes list aswell.
    auto formatter = [this, &identifier] ( const boost::smatch & r ) {
        std::string varMatching = r[1];
        if ( ( identifier == varMatching ) && ( syms.contains( varMatching ) )  ) {
            syms[varMatching].used = true;
            return syms[varMatching].value;
        } else {
            return varMatching; // original text
        }
    };

    // Iterate over dependecies and fill them
    for ( auto &sentPtr : *depsPtr ) {

        *sentPtr = boost::regex_replace( *sentPtr, VAR_REF, boost::ref(formatter));
    }
}



void ConfigurationParser::handleSentence( boost::smatch &result,
        const boost::filesystem::path &file, const std::size_t lineNumber ) {
    // we need to extract the sentence itself and the associated id.
    auto sent = std::make_shared<std::string>( result[1] ); // actual sentence
    boost::trim( *sent );

    if ( sents_.count( sent ) == 0 ) {
        sents_.insert( sent );
        map_[sent] = result[2];
        // now handle variables
        auto formatter = [this, &sent]( const boost::smatch & r ) {
            std::string identifier = r[1];
            if ( syms.contains( identifier ) ) {
                syms[identifier].used = true;
                return syms[identifier].value; // replace
            } else {
                syms.addDep( sent, identifier ); // forward declare
                return identifier;// keep original
            }
        };

        *sent = boost::regex_replace( *sent, VAR_REF, boost::ref(formatter));

    } else {
        //ignore new definition
        if ( warnLevel & WDuplicate  ) {
            *logger << "Warning: Duplicate sentence found @" << file.generic_string()
                    << ":" << lineNumber << "\nWill be ignored";
        }
    }
}