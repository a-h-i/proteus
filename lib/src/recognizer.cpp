#include "../recognizer.hpp"
#include "../recorder/microphone.hpp"
#include <chrono>
#include <cstring>
// 4 second ringBuffer
Recognizer::Recognizer( const std::string &grammar, const std::string &dict,
                        double SR, unsigned long FS ) : mic( this, SR, FS ), ringBuff( SR * 10 ), m(),
    SAMPLE_RATE( SR ), FRAME_SIZE( FS ) {
    // configure sphinx
    cmd_ln_t *config = cmd_ln_init( NULL, ps_args(), TRUE,
                                    "-fsg", grammar.c_str(),
                                    "-dict", dict.c_str(),
                                    NULL );
    ps = ps_init( config );
}





char *Recognizer::recogWord( const silencers::silencer_t &silen ) {
    // first we instruct the mic to start recording
    // WE must empty the buffer
    auto dummy = []( std::int16_t s ) {
        return true;
    };
    ringBuff.consume_all( dummy );
    mic.start();
    char *uttr = singleUttrHelper( silen );
    mic.stop();
    return uttr;
}

char *Recognizer::singleUttrHelper( const silencers::silencer_t &silen ) {
    const char *utt, *hyp;
    hyp = utt = nullptr;
    // signal start utterance processing
    ps_start_utt( ps, nullptr ); // creates a new utterance id

    // now we go into process mode
    // where we keep processing till we
    // detect silence.
    const std::size_t BUFF_SIZE = SAMPLE_RATE;
    std::vector<std::int16_t> backBuffer( BUFF_SIZE ); // a buffer for one second
    std::size_t readSoFar = 0;
    bool processedSomething = false;
    unsigned int times = 0;

    while ( true ) {
        do {
            // we process 1 second worth of data
            readSoFar += ringBuff.pop( &backBuffer[readSoFar], BUFF_SIZE - readSoFar );
            wait();
        } while ( readSoFar != BUFF_SIZE );

        if ( processedSomething && silen( &backBuffer[0], readSoFar ) ) {
            // 1 second of silence
            ps_end_utt( ps );
            hyp = ps_get_hyp( ps, nullptr, &utt ); // ignore score

            if ( hyp != nullptr ) {
                //allocate storage
                char *str = new char[std::strlen( hyp ) + 1];
                std::strcpy( str, hyp );
                return str;
            } else {
                return nullptr;
            }
        } else if ( (times >= 4) & (!processedSomething) ) {
            ps_end_utt( ps );
            return nullptr;
        } else {
            // process data
            processedSomething = true;
            ps_process_raw( ps, &backBuffer[0], readSoFar, FALSE, FALSE );
            readSoFar = 0;
        }
    }

    // end utterance

}
void Recognizer::wait() {
    std::unique_lock<std::mutex> lk( m );
    cv.wait( lk );
}
void Recognizer::notify() {
    if ( mic.isRecording() ) {
        cv.notify_one();
    }
}

void Recognizer::buffer( const std::int16_t *buff, std::size_t size ) {
    // perfectly safe
    // and lock free so as not to take valuable capture time !
    ringBuff.push( buff, size );
}

Recognizer::~Recognizer() {
   // ps_free( ps );
}