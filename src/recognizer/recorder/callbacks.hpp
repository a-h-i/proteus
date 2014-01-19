#pragma once
#include <portaudio.h>
namespace proteus {
namespace utility {
namespace callbacks {

int capturingCallback( const void *input,
                       void *output,
                       unsigned long frameCount,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData ) ;

}
}
}