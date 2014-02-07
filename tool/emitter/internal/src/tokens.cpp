#include "../tokens.hpp"

using namespace emitter::internal;
const boost::regex tokens::fileBlockBegin(
R"(\s*FILE\s*([a-zA-Z0-9.\\-]+)\s*BEGIN\s*)" ); // match[1] is name
const boost::regex tokens::fileBlockEnd( R"(\s*FILE END\s*)" );
const boost::regex tokens::wantTheDictionary( "THE_DICT" );
const boost::regex tokens::wantTheGrammar( "THE_GRAMMAR" );
const boost::regex tokens::idOfCommand(
 R"(ID_OF_COMMAND\(\s*([a-zA-Z0-9 ]+)\s*\))" ); // match[1] is command
const boost::regex tokens::beginExpandBlock( R"(\s*EXPAND_ME_BEGIN\s*)" );
const boost::regex tokens::endExpandBlock( R"(\s*EXPAND_ME_END\s*)" );
const boost::regex tokens::putCommandHere("COMMAND_EX");
const boost::regex tokens::putIDHere("ID_EX");