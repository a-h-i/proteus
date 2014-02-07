#pragma once
#include "parser_state.hpp"
#include <memory>
namespace emitter {
namespace internal {



class TemplateParser {
    ParserData data;
    std::unique_ptr<TemplateParserState> state;
public:
    TemplateParser( const std::string   &templateSrc,
                    const std::string   &dictionaryFileName,
                    const std::string &grammarFileName,
                    gen::sent_map_t const *const sentenceMap,
                    logger_ptr_t const logger );
    void parse();
};


}
}