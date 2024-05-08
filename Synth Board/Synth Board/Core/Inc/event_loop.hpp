#ifndef EVENTLOOP_HPP_
#define EVENTLOOP_HPP_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "InputHandler.hpp"

#ifdef __cplusplus

#include "Synthesizer.hpp"
#include "Oscillator.hpp"
#include "LowPassFilter.hpp"
#include "ADSREnvelope.hpp"
#include "AudioBuffer.hpp"

#define OUTPUT_DEVICE_HEADPHONE       2

extern Synthesizer synth;
extern Oscillator oscillator;
extern ADSREnvelope envelope;
extern LowPassFilter filter;

// Using https://barenakedembedded.com/how-to-use-cpp-with-stm32-hal/
void EventLoopCpp();   // C++ function to call into main event loop

extern "C" {
#endif
void EventLoopC();  // C function to call into C++ event loop from main
#ifdef __cplusplus
}
#endif

#endif /* EVENTLOOP_HPP_ */
