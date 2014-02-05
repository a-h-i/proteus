#pragma once
#include "recorder/microphone.hpp"
#include <list>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdint>
#include <condition_variable>
#include <pocketsphinx/pocketsphinx.h>


//  _________________________________
// < this is where the magic happens >
//  ---------------------------------
//         \   ^__^
//          \  (oo)\_______
//             (__)\       )\/\
//                 ||----w |
//                 ||     ||


class Prot {
    proteus::utility::Microphone mic;
    ps_decoder_t *ps;
    std::list<std::vector<std::int16_t> *> frames;
    std::mutex m, bufferM;
    std::condition_variable cv;
public:
    Prot( const std::string &grammar, const std::string &dict );
    const char *recogWord();
    void notify();
    void buffer(std::vector<std::int16_t> *buff);

};