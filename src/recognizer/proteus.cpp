#include "proteus.hpp"
#include "recorder/microphone.hpp"
#include "prot.hpp"

prot_t init_prot(const char * grammar, const char * dict) {
    return new Prot(grammar, dict);
}

const char * recog_word(prot_t p_) {
    Prot * p = reinterpret_cast<Prot *>(p_);
    return p->recogWord();
}