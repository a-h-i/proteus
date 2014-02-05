#ifdef DEBUG_ME_SOFTLY

#include "../ui/basicConsoleLogger.hpp"
#include "../generator/grammar.hpp"
#include "../generator/parser.hpp"
#include "catch.hpp"
#include <list>
#include <memory>
#include <string>



TEST_CASE( "NON OPTIMIZED GRAMMAR", "[grammar]" ) {
    std::shared_ptr<ILogger> logger = std::make_shared<BasicConsoleLogger>();
    const std::string simpleTest = "src/tests/simpletest.cfg";
    gen::parsing::ConfigurationParser p( simpleTest, logger, 0 );
    REQUIRE_FALSE( p.error() );
    gen::grammar::Grammar g( p.sentences() );
    *logger << "\nGrammar from file " << simpleTest << ":\n";
    gen::grammar::print( *logger, g );
}

#endif