#pragma once
#include "silencers.hpp"
#include "filters.hpp"
#include "recorder/microphone.hpp"

#include <list>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdint>
#include <condition_variable>
#include <pocketsphinx/pocketsphinx.h>
#include <boost/lockfree/spsc_queue.hpp>


//  _________________________________
// < this is where the magic happens >
//  ---------------------------------
//         \   ^__^
//          \  (oo)\_______
//             (__)\       )\/\
//                 ||----w |
//                 ||     ||


class Recognizer {
    proteus::utility::Microphone mic;
    ps_decoder_t *ps;
    boost::lockfree::spsc_queue<std::int16_t> ringBuff;
    std::mutex m;
    std::condition_variable cv;
    char *singleUttrHelper( const silencers::silencer_t &,  const filters::filter_t &  );
    void wait();
public:
    const double SAMPLE_RATE; // advised to be atlease 8k
    const unsigned long FRAME_SIZE;
    /****************************************
     * Throws PortAudio exception if failed *
     * to init microphone.                  *
     ****************************************/
    Recognizer( const std::string &grammar, const std::string &dict,
                double sampleRate,
                unsigned long frameSize );
    /***********************************
     * Provide a 'push to talk' method *
     ***********************************/
    char *recogWord( const silencers::silencer_t &, const filters::filter_t & );
    /**************************************************
     * Called by subjects this instance is observing. *
     * so far only microphones.                       *
     * ************************************************/
    void notify();
    /***************************
     * Adds data to buffer.    *
     * Operation is lock-free. *
     ***************************/
    void buffer( const std::int16_t *buff, std::size_t size );
    ~Recognizer();
};


