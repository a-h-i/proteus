#pragma once


typedef void * prot_t;
extern "C" {
    // must call before any other function
    prot_t prot_init(const char * grammar, const char * dict);
    // 'push to talk' style
    // freeing returned pointer is responsibility of user
    char * recog_word(prot_t);
    // call when finished to release resources
    void prot_free(prot_t);

}