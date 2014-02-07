#include "../expansion_macro_state.hpp"
#include "../file_block_state.hpp"
#include "../../repeating_emitter.hpp"
#include "../tokens.hpp"
#include <sstream>
using namespace emitter::internal;


std::unique_ptr<TemplateParserState> ExpansionMacroState::parse(
    ParserData &data ) {
    std::ostringstream buffer;

    for ( ; std::getline( data.src, data.sd.lineBuffer ); ) {
        if ( boost::regex_match( data.sd.lineBuffer, tokens::endExpandBlock ) ) {
            data.sd.seq.emplace_back( new RepeatingEmitter( data.sd.outStream, buffer.str(),
                                      data.grammarFileName, data.dictionaryFileName, data.sentenceMap ) );
            return std::unique_ptr<TemplateParserState>( new FileBlockState );
        } else {
            buffer << data.sd.lineBuffer << '\n';
        }
    }

    *data.logger <<
                 "\nWarning: Failed to match EXPANSION_BLOCK CLOSING templateFile: " <<
                 data.templateSrc;
    return std::unique_ptr<TemplateParserState>( new FileBlockState );
}