#include "../replace_vars.hpp"
#include "../tokens.hpp"

std::string emitter::internal::replaceVariables( const std::string &text, const std::string &grammarFile,
                              const std::string &dictFile, const gen::sent_map_t  * const map ){
    // first fill in grammar file if needed
    std::string out = boost::regex_replace(text, tokens::wantTheGrammar, grammarFile);
    // now fill in dict file
    out = boost::regex_replace(out, tokens::wantTheDictionary, dictFile);
    // handle ID_OF
    auto formatter = [map](const boost::smatch &result) -> std::string {
        if(map->count(result[1]) != 0) {
            return map->at(result[1]);
        }else {
            return std::string("COULD NOT FIND THIS: ") + result[0];
        }
    };

    return boost::regex_replace(out, tokens::idOfCommand, formatter);

}