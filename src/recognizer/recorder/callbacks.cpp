#include "../prot.hpp"
#include "callbacks.hpp"
#include "microphone.hpp"
#include <cstdint>
using namespace proteus::utility;

int callbacks::capturingCallback( const void *input,
                                  void *output,
                                  unsigned long frameCount,
                                  const PaStreamCallbackTimeInfo *timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData ) {

 Prot * p = reinterpret_cast<Prot *>(userData);
 const std::int16_t * iptr = reinterpret_cast<const std::int16_t *>(input);
 p->buffer( iptr, frameCount); // lockfree 
 p->notify();
 return 0; // continue processing

}