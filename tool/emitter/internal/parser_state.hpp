#pragma once
#include "../emitter.hpp"
#include "../../ui/ilogger.hpp"
#include <list>
#include <string>
#include <memory>
#include <fstream>
namespace emitter {
namespace internal {


struct StateData {
    std::string lineBuffer;
    std::shared_ptr<std::ofstream> outStream;  // used for file block, file blocks are non recursive
    std::list<std::unique_ptr<Emitter>> seq;
    bool done, matchedFileClose;
    StateData() : done( false ) {}
    StateData( const StateData & ) = delete;
    StateData &operator=( const StateData & ) = delete;
};

struct ParserData {
    const std::string templateSrc;
    const std::string dictionaryFileName;
    const std::string grammarFileName;
    gen::sent_map_t const *const sentenceMap;
    logger_ptr_t const logger;
    std::ifstream src;
    StateData sd; // for internal use by states
    ParserData( const std::string   &templateSrc,
                const std::string   &dictionaryFileName,
                const std::string &grammarFileName,
                gen::sent_map_t const *const sentenceMap,
                logger_ptr_t const logger ) : templateSrc( templateSrc ),
        dictionaryFileName( dictionaryFileName ), grammarFileName( grammarFileName ),
        sentenceMap( sentenceMap ), logger( logger ), src( templateSrc ),
        sd()  {}
};

struct TemplateParserState {
    virtual std::unique_ptr<TemplateParserState> parse( ParserData & ) = 0;
    bool done( ParserData &data ) {
        return data.sd.done;
    }
    virtual ~TemplateParserState() {};
};


}
}