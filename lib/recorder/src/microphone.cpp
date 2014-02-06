#include "../microphone.hpp"
#include "../../recognizer.hpp"
#include <string>

using namespace proteus::utility;


Microphone::Microphone(Recognizer *userData, const double sampleRate,
                        const unsigned long framesPerBuffer ) : sampleRate_( sampleRate ),
    framesPerBuffer_( framesPerBuffer ), recording(false) {
    PaError err = Pa_Initialize();

    if ( err != paNoError ) {
        throw proteus::exceptions::PortAudio(
            std::string( "PortAudio Error Code: " ) + Pa_GetErrorText( err ) );
    }

    /******************************************************************************
     *  One input stream, 0 output streams, signed 16-bit samples, ..., userData. *
     ******************************************************************************/
    err = Pa_OpenDefaultStream( &stream, 1, 0, paInt16, sampleRate_,
                                   framesPerBuffer_, callbacks::capturingCallback, userData );

    if ( err != paNoError ) {
        throw proteus::exceptions::PortAudio(
            std::string( "PortAudio Error Code: " ) + Pa_GetErrorText( err ) );
    }

}


void Microphone::start() {
    if(!recording) {
        recording = true;
        Pa_StartStream(stream);
    }
}

void Microphone::stop(){
    if(recording) {
        recording = false;
        Pa_StopStream(stream);
    }
}


Microphone::~Microphone() {
    Pa_CloseStream(stream);
    Pa_Terminate();
}