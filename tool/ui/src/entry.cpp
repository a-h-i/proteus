#ifdef DEBUG_ME_SOFTLY
#define CATCH_CONFIG_MAIN
#include "../../tests/catch.hpp"

#else

#include "../basicConsoleLogger.hpp"
#include "../../common/exceptions.hpp"
#include "../../common/utility.hpp"
#include "../../generator/grammar.hpp"
#include "../../generator/parser.hpp"
#include <list>
#include <bitset>
#include <cstdio>
#include <memory>
#include <vector>
#include <iterator>
#include <iostream>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace bfs = boost::filesystem;
namespace bp = boost::process;

#define OPTIONS_BITS 2
#define EXEC_DIR_BIT 0
#define CONFIGURATION_BIT 1
#define GRAMMAR_NAME_LENGTH 15

class Options {
    std::bitset<OPTIONS_BITS> bits;
    bfs::path execDir;
    std::list<bfs::path> confFiles;
    gen::parsing::ConfigurationParser::warningLevel_t warnLevel;
public:
    Options() : warnLevel( 0 ) {}
    void setExecDir( const std::string &p ) {
        execDir = p;
        execDir.remove_filename();
        bits.set( EXEC_DIR_BIT );
    }
    void addConfFile( const std::string &p ) {
        confFiles.emplace_back( p );
        bits.set( CONFIGURATION_BIT );
    }
    bool checkComp( std::size_t i ) {
        return bits.test( i );
    }
    void setWarnLevel( const decltype( warnLevel ) &level ) {
        warnLevel |= level;
    }
    bool good() {
        return bits.all();
    }
    gen::parsing::ConfigurationParser createParser( const std::shared_ptr<ILogger>
            &logger ) {
        gen::parsing::ConfigurationParser p( logger, warnLevel );

        for ( auto &file : confFiles ) {
            p.parseFile( file );
        }

        return p;
    }
};

static Options parseCommands( std::list<std::string> & );
static std::list<std::string> convertArgs( const int argc,
        const char *const *const argv );
static void retriveConfFiles( Options &, std::list<std::string> & );
static void retriveWarningSetting( Options &, std::list<std::string> & );

int main( int argc, const char *argv[] ) {
    std::shared_ptr<ILogger> logger = std::make_shared<BasicConsoleLogger>();
    std::list<std::string> args = convertArgs( argc, argv );
    Options opts;

    try {
        opts = parseCommands( args );
    } catch ( proteus::exceptions::InvalidArgs &e ) {
        *logger << "Unrecognized arguments:\n";

        for ( auto &arg : args ) {
            *logger << arg << ' ';
        }

        *logger << '\n';
        return -1;
    }

    if ( opts.good() ) {
        try {
            // Now parse configuration file to create grammar
            auto parser = opts.createParser( logger );

            if ( parser.error() ) {
                // parser should have logged errors
                *logger << "\nConfiguration Failed\n";
                return -1;
            }


            // now create grammar
            gen::grammar::Grammar g( parser.sentences() );

            g.optimize( gen::optimizers::simpleFactoringOptimizer );

            std::string randomName;

            do {
                randomName = gen::generateRandomString( GRAMMAR_NAME_LENGTH );
                // generate a not in use file name (relative to working directory)
            } while ( ( bfs::exists( randomName + ".jsgf" ) )
                      || ( bfs::exists( randomName + ".fsg" ) ) );

            std::string jsgfGrammar = gen::grammar::createJSGF( g, randomName );
            const std::string jsgfName = randomName + ".jsgf";
            const std::string fsgName = randomName + ".fsg";
            std::ofstream outFile( jsgfName );
            outFile << jsgfGrammar;
            outFile.flush();
            // Now we need to convert the jsgf to a fsg
            const std::string converter( "sphinx_jsgf2fsg" );

            system( ( converter + " -jsgf " + jsgfName + " -fsg " + fsgName ).c_str() );

            // now we have a fsg thats good to go.
            std::remove(jsgfName.c_str()); // delete jsgf
            return 0;
        } catch ( proteus::exceptions::FileError &e ) {
            *logger << e.what();
            return -1;
        }
    } else {
        if ( !opts.checkComp( EXEC_DIR_BIT ) ) {
            *logger << "\nFatal Error: Could not locate my own executable !"
                    << "\nPlease report this incident including OS info\n";
        }

        if ( !opts.checkComp( CONFIGURATION_BIT ) ) {
            *logger << "\nFatal Error: No cinfiguration file provided\n"
                    << "Configuration files must end with .cfg\n";
        }

        return -1;
    }

}

static std::list<std::string> convertArgs( const int argc,
        const char *const *const argv ) {
    // preserves order
    std::list<std::string> args;

    for ( int i = 0; i < argc; i++ ) {
        args.emplace_back( argv[i] );
    }

    return args;
}
static Options parseCommands( std::list<std::string> &args ) {
    Options opts;

    if ( args.size() >= 1 ) {
        using namespace gen::parsing;

        // execDir is first argument
        opts.setExecDir ( args.front() );
        args.pop_front();
        // get configuration files
        retriveConfFiles( opts, args );
        retriveWarningSetting( opts, args );

        if ( args.size() != 0 ) {
            throw proteus::exceptions::InvalidArgs( "Unrecognized command line options" );
        }

    }

    return opts;
}


template<typename It, typename Cont>
It eraseItr( It it, Cont &c ) {
    auto prev = std::prev( it, 1 );
    c.erase( it );
    return prev;
}

static void retriveWarningSetting( Options &opts,
                                   std::list<std::string> &args ) {
    using namespace gen::parsing;

    for ( auto it = args.begin(); it != args.end(); ++it ) {
        if ( *it == "-Wall" ) {
            it = eraseItr( it, args );
            opts.setWarnLevel( ConfigurationParser::Wall );
        } else if ( *it == "-WDuplicate" ) {
            it = eraseItr( it, args );
            opts.setWarnLevel( ConfigurationParser::WDuplicate );
        } else if ( *it == "-WUnused" ) {
            it = eraseItr( it, args );
            opts.setWarnLevel( ConfigurationParser::WUnused );
        } else if ( *it == "-WReAssign" ) {
            it = eraseItr( it, args );
            opts.setWarnLevel( ConfigurationParser::WReAssign );
        }
    }

}

static void retriveConfFiles( Options &opts, std::list<std::string> &args ) {
    for ( auto it = args.begin(); it != args.end(); ++it ) {
        if ( boost::ends_with( *it, ".cfg" ) ) {
            opts.addConfFile( *it );
            it = eraseItr( it, args );
        }
    }
}

#endif