#pragma once
#include <portaudio.h>
#include "../exceptions.hpp"
#include "callbacks.hpp"
class Recognizer;
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
    Microphone(Recognizer *, const double sampleRate = 8192.0, const unsigned long framesPerBuffer = 4096);
    double sampleRate() { return sampleRate_;}
    unsigned long framesPerBuffer(){return framesPerBuffer_;}
    void start();
    void stop();
    bool isRecording() {return recording;}
    ~Microphone();
};

}
}