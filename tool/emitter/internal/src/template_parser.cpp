
#include "../template_parser.hpp"
#include "../initial_state.hpp"
using namespace emitter::internal;


TemplateParser::TemplateParser( const std::string   &templateSrc,
                                const std::string   &dictionaryFileName,
                                const std::string &grammarFileName,
                                gen::sent_map_t const *const sentenceMap,
                                logger_ptr_t const logger ) : data( templateSrc, dictionaryFileName,
                                grammarFileName, sentenceMap, logger ), state( new ParserInitialState ) {}


void TemplateParser::parse() {

    while ( !state->done( data ) ) {
        std::unique_ptr<TemplateParserState> newState = state->parse( data );
        state.swap( newState );
    }

    for(auto &eptr : data.sd.seq) {
        eptr->emit();
    }

   
}