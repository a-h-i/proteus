#pragma once
#include "parser_state.hpp"

namespace emitter {
namespace internal {
struct ExpansionMacroState : TemplateParserState {
    std::unique_ptr<TemplateParserState> parse( ParserData & ) override;
};
}
}