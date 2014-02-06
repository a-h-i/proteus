#include "../plain_text_emission.hpp"


void emitter::PlainTextEmission::emit() {
    if(file->good()) {
        *file << text;
    }else {
        throw proteus::exceptions::EmissionError("File not good - PlainTextEmission");
    }
}