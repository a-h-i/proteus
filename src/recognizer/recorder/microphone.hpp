#pragma once
#include <portaudio.h>
#include "../common/exceptions.hpp"
#include "callbacks.hpp"
namespace proteus {
namespace utility {


/*****************************************
 * Class representing a capture device.  *
 * Utilizes PortAudio for cross-platform *
 * Audio Capture.                        *
 *****************************************/
class Microphone {
    friend int callbacks::capturingCallback( const void *input,
                       void *output,
                       unsigned long frameCount,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData ) ;
    PaStream *stream;
    const double sampleRate_;
    const unsigned long framesPerBuffer_;
    bool recording;
public:
    /****************************************
     * Throws PortAudio exception if failed *
     * to Initialize PA.                    *
     ****************************************/
    Microphone(const double sampleRate = 8000.0, const unsigned long framesPerBuffer = 200);
    double sampleRate() { return sampleRate_;}
    unsigned long framesPerBuffer(){return framesPerBuffer_;}
    start();
    stop();
    ~Microphone();
};

}
}