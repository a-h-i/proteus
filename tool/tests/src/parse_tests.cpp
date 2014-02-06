#ifdef DEBUG_ME_SOFTLY

#include "../catch.hpp"
#include "../../ui/basicConsoleLogger.hpp"
#include "../../generator/parser.hpp"
#include <memory>
#include <string>


TEST_CASE( "Single File parsing", "[parser]" ) {
    std::unique_ptr<ILogger> logger(new BasicConsoleLogger);
    const std::string simpleTest = "tool/tests/simpletest.cfg";
    const std::string varTest = "tool/tests/vartest.cfg";
    SECTION( "NO WARRNING" ) {
        *logger << '\n' << simpleTest << ":\n";
        gen::parsing::ConfigurationParser p( simpleTest, logger.get(), 0 );
        gen::parsing::print( *logger, p );
        REQUIRE_FALSE( p.error() );
    }

    SECTION( "WARNINGS ENABLED" ) {
        *logger << '\n' << simpleTest << ":\n";
        gen::parsing::ConfigurationParser p( simpleTest, logger.get(),
                                             gen::parsing::ConfigurationParser::Wall );
        gen::parsing::print( *logger, p );
        REQUIRE_FALSE( p.error() );
    }

    SECTION( "NON EXISTING FILE" ) {
        REQUIRE_THROWS_AS( gen::parsing::ConfigurationParser( "tool/tests/notafile.cfg",
                           logger.get() ), proteus::exceptions::FileError );
    }

    SECTION( "File With variables" ) {
        *logger << '\n' << varTest << ":\n";
        gen::parsing::ConfigurationParser p( varTest, logger.get(),
                                             gen::parsing::ConfigurationParser::Wall );
        gen::parsing::print( *logger, p );
        REQUIRE_FALSE( p.error() );
    }
}

#endif