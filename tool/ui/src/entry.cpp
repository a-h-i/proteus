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
#include <memory>





#define OPTIONS_BITS 2
#define EXEC_DIR_BIT 0
#define CONFIGURATION_BIT 1
#define GRAMMAR_NAME_LENGTH 15


class Options {
    std::bitset<OPTIONS_BITS> bits;
    bfs::path execDir;
    std::list<bfs::path> confFiles;
    gen::parsing::ConfigurationParser::warningLevel_t warnLevel;
    std::string tempFile;
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
    gen::parsing::ConfigurationParser createParser( const logger_ptr_t
            &logger ) {
        gen::parsing::ConfigurationParser p( logger, warnLevel );

        for ( auto &file : confFiles ) {
            p.parseFile( file );
        }

        return p;
    }
    void setTempFile( const std::string &str ) {
        tempFile = str;
    }
    void genAPI( const std::unordered_map<gen::sentence_t, gen::id_t>  map,
                 const std::string &dictFile, const std::string &gramFile ) {
        if ( !tempFile.empty() ) {

            emitter::createSequence( tempFile, dictFile, gramFile, &map, logger.get() );

        }
    }
};


void handleConfiguration(logger_ptr_t logger, std::unordered_multimap<OptionKey, std::string> &args) {
    using namespace ui;
    using cp = gen::parsing::ConfigurationParser;    
    cp::warningLevel_t warnLevel = cp::DEFAULT_WARN;
    auto warnRange = args.equal_range(OptionKey::CONF_WARNINGS);
    while(warnRange.first != warnRange.second) {
        if(*warnRange.first == "-Wall") {
            warnLevel |= cp::Wall;
        }else if (*warnRange.first == "-WDuplicate") {
            warnLevel |= cp::WDuplicate;
        }else if (*warnRange.first == "-WUnused") {
            warnLevel |= cp::WUnused;
        }else if (*warnRange.first == "WReAssign") {
            warnLevel |= cp::WReAssign;
        }
        ++warnRange.first;
    }
    cp parser(logger, warnLevel);
    auto confRange = args.equal_range(OptionKey::CONFIGURATION_FILES);
    while(confRange.first != confRange.second) {
        parser.parseFile(*confRange.first);
        ++confRange.first;
    }
    // now we generate the grammar
       
}



int main( int argc, const char *argv[] ) {
    using namespace ui;
    using namespace proteus::exceptions;
    try {
        std::unique_ptr<ILogger> logger ( new BasicConsoleLogger() );
        std::unordered_multimap<OptionKey, std::string> args = convertArgs( argc, argv );
        if(args.count(CONFIGURATION_FILES) == 0) {
            std::cerr << "Fatal Error: No configuration files specified.\n";
            return -1;
        }
        
        const std::string dictFileName = "dict.dict";
        std::string fsgGrammarFileName;


    }catch(InvalidArgs &e) {
        std::cerr << "Unknown Option/Argument : " << e.what()
                  << "\nUse proteus -h for help\n"
    }catch(NoWorkException &e) {
        // nothing to do
        return 0;
    }
}
#endif