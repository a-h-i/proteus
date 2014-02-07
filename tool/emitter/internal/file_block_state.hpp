#pragma once
#include "parser_state.hpp"
#include <memory>
#include <fstream>

namespace emitter {
namespace internal {

/************************
 * Parses A File Block  *
 *                      *
 ************************/
struct FileBlockState : TemplateParserState {
    FileBlockState(const std::string & targetFileState, ParserData &data){
        data.sd.outStream = std::make_shared<std::ofstream>(targetFileState);
        
    }
     FileBlockState() = default;

    std::unique_ptr<TemplateParserState> parse( ParserData & ) override;

};


} 
}