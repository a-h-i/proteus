#include "../plain_text_emitter.hpp"


void emitter::PlainTextEmitter::emit() {
    if(file->good()) {
        *file << text;
    }else {
        throw proteus::exceptions::EmitterError("File not good - PlainTextEmitter");
    }
}