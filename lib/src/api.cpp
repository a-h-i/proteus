#include "../proteus.hpp"
#include "../recorder/microphone.hpp"
#include "../recognizer.hpp"
#include "../silencers.hpp"
#include <cmath>

prot_t prot_init( const char *grammar, const char *dict ) {
    return new Recognizer( grammar, dict, 8192.0,
                           4096 ); // sample rate and frame size
    // yes I pulled these numbers from there
}

char *recog_word( prot_t p_ ) {
    // we want the threshold to be 42ish decibels
    // silencer expects a decibel threshold
    // with 0db being max thus we want -42 corresponding to about 0.01 of amplitude
    static silencers::silencer_t silen = silencers::RMSSilencer(
            -42.0 ); // I will have you know this is a well researched number
    Recognizer *p = reinterpret_cast<Recognizer *>( p_ );
    return p->recogWord( silen );
}

void prot_free( prot_t p_ ) {
    Recognizer *p = reinterpret_cast<Recognizer *>( p_ );
    delete p;
}