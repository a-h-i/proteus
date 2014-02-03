#pragma once
#include "../ui/basicConsoleLogger.hpp"
#include "../generator/parser.hpp"
#include <memory>
#include <string>
#include <iostream>

TEST_CASE( "Single File parsing", "[parser]" ) {
    std::shared_ptr<ILogger> logger = std::make_shared<BasicConsoleLogger>();
    const std::string simpleTest = "src/tests/simpletest.cfg";
    SECTION( "NO WARRNING" ) {

        gen::parsing::ConfigurationParser p( simpleTest, logger, 0 );
        *logger << "\nsrc/tests/simpletest.cfg:\n";
        gen::parsing::printConfiguration( *logger, p );
        REQUIRE_FALSE( p.error() );
    }

    SECTION( "WARNINGS ENABLED" ) {
        gen::parsing::ConfigurationParser p( simpleTest, logger,
                                             gen::parsing::ConfigurationParser::Wall );
        *logger << "\nsrc/tests/simpletest.cfg:\n";
        gen::parsing::printConfiguration( *logger, p );
        REQUIRE_FALSE( p.error() );
    }

    SECTION( "NON EXISTING FILE" ) {
        REQUIRE_THROWS_AS( gen::parsing::ConfigurationParser( "src/tests/notafile.cfg",
                           logger ), FileError );
    }
}