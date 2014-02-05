#ifdef DEBUG_ME_SOFTLY

#include "catch.hpp"
#include "../ui/basicConsoleLogger.hpp"
#include "../generator/grammar.hpp"
#include "../generator/parser.hpp"
#include <list>
#include <memory>
#include <string>
#include <fstream>


TEST_CASE( "NON OPTIMIZED GRAMMAR", "[grammar]" ) {
    std::shared_ptr<ILogger> logger = std::make_shared<BasicConsoleLogger>();
    const std::string simpleTest = "src/tests/simpletest.cfg";
    gen::parsing::ConfigurationParser p( simpleTest, logger, 0 );
    REQUIRE_FALSE( p.error() );
    gen::grammar::Grammar g( p.sentences() );
    *logger << "\nGrammar from file " << simpleTest << ":\n";
    gen::grammar::print( *logger, g );
}

TEST_CASE( "OPTIMIZED GRAMMAR", "[grammar]" ) {
    std::shared_ptr<ILogger> logger = std::make_shared<BasicConsoleLogger>();
    const std::string simpleTest = "src/tests/simpletest.cfg";
    gen::parsing::ConfigurationParser p( simpleTest, logger, 0 );
    REQUIRE_FALSE( p.error() );
    gen::grammar::Grammar g( p.sentences() );
    *logger << "\nGrammar from file " << simpleTest << ":\n";
    REQUIRE_NOTHROW( g.optimize( gen::optimizers::simpleFactoringOptimizer ) );
    gen::grammar::print( *logger, g );

}

TEST_CASE( "JSGF OUTPUT", "[grammar]" ) {
    std::shared_ptr<ILogger> logger = std::make_shared<BasicConsoleLogger>();
    const std::string simpleTest = "src/tests/simpletest.cfg";

    SECTION( "NON OPTIMIZED GRAMMAR" ) {
        gen::parsing::ConfigurationParser p( simpleTest, logger, 0 );
        REQUIRE_FALSE( p.error() );
        gen::grammar::Grammar g( p.sentences() );
        *logger << "\nNon Omptimized:\n";
        std::string out = gen::grammar::createJSGF( g, "nonoptSimple" );
        *logger << out;
        std::ofstream outFile( "nonoptSimple.jsgf" );
        outFile << out;
    }

    SECTION( "OPTIMIZED GRAMMAR" ) {
        gen::parsing::ConfigurationParser p( simpleTest, logger, 0 );
        REQUIRE_FALSE( p.error() );
        gen::grammar::Grammar g( p.sentences() );
        REQUIRE_NOTHROW( g.optimize( gen::optimizers::simpleFactoringOptimizer ) );
        *logger << "\nOmptimized:\n";
        std::string out = gen::grammar::createJSGF( g, "optSimple" );
        *logger << out;
        std::ofstream outFile( "optSimple.jsgf" );
        outFile << out;
    }
}

#endif