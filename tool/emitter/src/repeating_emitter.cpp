#include "../repeating_emitter.hpp"
#include "../internal/tokens.hpp"
#include "../internal/replace_vars.hpp"
void emitter::RepeatingEmitter::emit() {

    if(file->good()) {
        for(auto &pair: *map) {
            // first replace the itr variables
                std::string out = boost::regex_replace(mold, internal::tokens::putCommandHere, pair.first );
                out = boost::regex_replace(out, internal::tokens::putIDHere, pair.second);
                // ok now we need to replace variables
                out = internal::replaceVariables(out, grammarFile, dictFile, map);
                *file << out;                
        }
   
    }else {
        throw proteus::exceptions::EmitterError("File not good - RepeatingEmitter");
    }
}