#include "../initial_state.hpp"
#include "../file_block_state.hpp"
#include "../tokens.hpp"
using namespace emitter::internal;




std::unique_ptr<TemplateParserState> ParserInitialState::parse(
    ParserData &data ) {
    if(!data.sd.done) {
    for(; std::getline( data.src, data.sd.lineBuffer); ) {
        boost::smatch result;
        if ( boost::regex_match( data.sd.lineBuffer, result, tokens::fileBlockBegin ) ) {
            data.sd.matchedFileClose = false;
            return std::unique_ptr<TemplateParserState> (new FileBlockState(result[1], data)); // pass File name
        }
    }
        data.sd.done = true;
    }else {
        *data.logger << "\nFatal Error InitialState parsing while done\n";
        
    }
    return std::unique_ptr<TemplateParserState> (new ParserInitialState());
    
}