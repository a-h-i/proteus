#ifdef DEBUG_ME_SOFTLY
#define CATCH_CONFIG_MAIN
#include "../../tests/catch.hpp"

#else
#include "../../emitter/template.hpp"
#include "../basicConsoleLogger.hpp"
#include "../../common/exceptions.hpp"
#include "../../common/utility.hpp"
#include "../../generator/grammar.hpp"
#include "../../generator/parser.hpp"
#include "../arguments.hpp"
#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>


constexpr std::size_t GRAMMAR_NAME_LENGTH = 12;

using namespace ui;
static std::string handleConfiguration( logger_ptr_t logger,
                          std::unordered_multimap<OptionKey, std::string> &args, emitter::sent_map_t & finalMap ) {
    using cp = gen::parsing::ConfigurationParser;
    cp::warningLevel_t warnLevel = cp::DEFAULT_WARN;
    auto warnRange = args.equal_range( OptionKey::CONF_WARNINGS );

    while ( warnRange.first != warnRange.second ) {
        if ( (*warnRange.first).second == "-Wall" ) {
            warnLevel |= cp::Wall;
        } else if ( (*warnRange.first).second == "-WDuplicate" ) {
            warnLevel |= cp::WDuplicate;
        } else if ( (*warnRange.first).second == "-WUnused" ) {
            warnLevel |= cp::WUnused;
        } else if ( (*warnRange.first).second == "-WReAssign" ) {
            warnLevel |= cp::WReAssign;
        }

        ++warnRange.first;
    }

    cp parser( logger, warnLevel );
    auto confRange = args.equal_range( OptionKey::CONFIGURATION_FILES );

    while ( confRange.first != confRange.second ) {
        parser.parseFile( (*confRange.first).second );
        ++confRange.first;
    }

    // now we generate the grammar
    gen::grammar::Grammar gram( parser.sentences() );
    gram.optimize( gen::optimizers::simpleFactoringOptimizer );
    namespace bfs = boost::filesystem;
    std::string randomName;
    do {
        randomName = gen::generateRandomString( GRAMMAR_NAME_LENGTH );
        // generate a not in use file name (relative to working directory)
    } while ( ( bfs::exists( randomName + ".jsgf" ) )
              || ( bfs::exists( randomName + ".fsg" ) ) );
    std::string jsgfGrammar = gen::grammar::createJSGF( gram, randomName );
    const std::string jsgfName = randomName + ".jsgf";
    const std::string fsgName = randomName + ".fsg";
    std::ofstream outFile( jsgfName );
    outFile << jsgfGrammar;
    outFile.flush();
    const std::string converter( "sphinx_jsgf2fsg" );
    system( ( converter + " -jsgf " + jsgfName + " -fsg " + fsgName ).c_str() );
    std::remove(jsgfName.c_str());
    finalMap = parser.getFinalMap();
    return fsgName;
}



int main( int argc, const char *argv[] ) {
    using namespace proteus::exceptions;
    
    try {
        std::unique_ptr<ILogger> logger ( new BasicConsoleLogger() );
        
        std::unordered_multimap<OptionKey, std::string> args = getConfiguration(convertArgs( argc, argv));

        if ( args.count( OptionKey::CONFIGURATION_FILES ) == 0 ) {
            std::cerr << "Fatal Error: No configuration files specified.\n";
            return -1;
        }
        emitter::sent_map_t finalMap;
        const std::string dictFileName = "dict.dict";
        std::string fsgGrammarFileName = handleConfiguration(logger.get(), args, finalMap);
        // now we need to generate template
        
        auto tempFileRange = args.equal_range(OptionKey::TEMPLATE_FILES);
        while(tempFileRange.first != tempFileRange.second) {
            emitter::processTemplate( (*tempFileRange.first).second, dictFileName, fsgGrammarFileName, &finalMap, logger.get() );
            ++tempFileRange.first;
        }
        
        return 0;
    } catch ( InvalidArgs &e ) {
        std::cerr << "Unknown Option/Argument : " << e.what()
                  << "\nUse proteus -h for help\n";
        return -1;
    } catch ( NoWorkException &e ) {
        // nothing to do
        return 0;
    }
}
#endif