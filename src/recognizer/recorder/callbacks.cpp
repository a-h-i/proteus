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
 std::vector<std::int16_t> * buff = new std::vector<std::int16_t>();
 buff->reserve(frameCount);
 const std::int16_t * iptr = reinterpret_cast<const std::int16_t *>(input);
 for(unsigned long i =0 ; i < frameCount ; i++) {
    buff->push_back(iptr[i]);
 }
 p->buffer(buff);
 p->notify();
 return 0; // continue processing

}