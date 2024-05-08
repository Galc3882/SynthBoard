#ifndef INC_INPUTHANDLER_HPP_
#define INC_INPUTHANDLER_HPP_

#include "event_loop.hpp"
#include "ws2812-spi.hpp"

// Analog Inputs
#define ANALOG_INPUTS 8  // Number of ADC channels
#define DIGITAL_INPUTS 16

#define ADC1_BUFFER_SIZE (ANALOG_INPUTS * sizeof(uint8_t))
extern ADC_HandleTypeDef hadc1;
extern uint8_t adc1Buffer[ANALOG_INPUTS]; // Buffer to hold ADC conversion results

#define INPUT_DELAY 0.01 // In seconds
extern float nextInputTime;

extern uint8_t analogInputs[ANALOG_INPUTS];
extern bool digitalInputs[DIGITAL_INPUTS];

void getInputs();

#endif /* INC_INPUTHANDLER_HPP_ */
