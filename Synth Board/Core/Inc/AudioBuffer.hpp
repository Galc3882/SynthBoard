#ifndef INC_AUDIOBUFFER_HPP_
#define INC_AUDIOBUFFER_HPP_

#include "event_loop.hpp"

#define AUDIO_BUFFER_SIZE 256  // Size of the audio buffer
extern uint16_t audioBuffer[AUDIO_BUFFER_SIZE * 2];  // Double buffering
extern bool bufferHalf;  // Which half of the buffer is currently used
extern float time;

// Fill one half of the audio buffer with synthesized audio samples
void fillAudioBuffer();

#endif /* INC_AUDIOBUFFER_HPP_ */
