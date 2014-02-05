#pragma once


typedef void * prot_t;
extern "C" {
    prot_t init_prot(const char * grammar, const char * dict);
    // fully ended
    // freeing returned pointer is responsibility of user
    const char * recog_word(prot_t);

}