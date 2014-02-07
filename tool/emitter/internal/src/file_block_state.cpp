#include "../../plain_text_emitter.hpp"
#include "../file_block_state.hpp"
#include "../replace_vars.hpp"
#include "../tokens.hpp"
#include "../expansion_macro_state.hpp"
#include "../initial_state.hpp"
#include <sstream>

using namespace emitter::internal;

std::unique_ptr<TemplateParserState> FileBlockState::parse( ParserData &data ) {
    std::ostringstream buffer;

    for ( std::string line; std::getline( data.src, line ); ) {
        if ( boost::regex_match( line, tokens::fileBlockEnd ) ) {
            data.sd.seq.emplace_back( new PlainTextEmitter( data.sd.outStream,
                                      replaceVariables( buffer.str(), data.grammarFileName, data.dictionaryFileName,
                                              data.sentenceMap ) ) );
            data.sd.matchedFileClose = true;
            // we are done with file go back to initial state
            return std::unique_ptr<TemplateParserState>(new ParserInitialState);
        } else if ( boost::regex_match( line, tokens::beginExpandBlock ) ) {
            // switch to expand block
            data.sd.seq.emplace_back( new PlainTextEmitter( data.sd.outStream,
                                      replaceVariables( buffer.str(), data.grammarFileName, data.dictionaryFileName,
                                              data.sentenceMap ) ) );
            return std::unique_ptr<TemplateParserState>( new ExpansionMacroState );
        } else {
            buffer << line << '\n'; // buffer text
        }
    }
    data.sd.seq.emplace_back( new PlainTextEmitter( data.sd.outStream,
                                      replaceVariables( buffer.str(), data.grammarFileName, data.dictionaryFileName,
                                              data.sentenceMap ) ) );
    // get line failed
    return std::unique_ptr<TemplateParserState>( new ParserInitialState );
}