#pragma once
#include "parser_state.hpp"
#include <string>

namespace emitter {
namespace internal {
    struct ParserInitialState: TemplateParserState {
        std::unique_ptr<TemplateParserState> parse( ParserData & ) override;
    };
}
}