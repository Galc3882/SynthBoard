#include "InputHandler.hpp"

uint8_t adc1Buffer[ANALOG_INPUTS];
float nextInputTime = 0;
uint8_t analogInputs[ANALOG_INPUTS] = { 255, 65 * 0, 0, 0, 255, 0, 255, 1
		+ 32 * 5 };
// volume waveform attack decay sustain release filter harmonics
bool digitalInputs[DIGITAL_INPUTS];

GPIO_TypeDef *rowPorts[4] = { GPIOE, GPIOE, GPIOE, GPIOE };
uint16_t rowPins[4] = { GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11 };

GPIO_TypeDef *colPorts[4] = { GPIOB, GPIOB, GPIOB, GPIOE };
uint16_t colPins[4] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_7 };

// LEDs remapped because they snake around the board and make the ordering weird
uint8_t ledButtonMap[16] = { 12, 11, 4, 3, 14, 9, 6, 1, 13, 10, 5, 2, 15, 8, 7, 0 };

uint8_t ledColors[16][3] =
{
        {127, 0, 255},
        {31, 0, 255},
        {127, 255, 0},
        {223, 255, 0},
        {255, 0, 191},
        {0, 159, 255},
        {0, 255, 63},
        {255, 95, 0},
        {223, 0, 255},
        {0, 63, 255},
        {31, 255, 0},
        {255, 191, 0},
        {255, 0, 95},
        {0, 255, 255},
        {0, 255, 159},
        {255, 0, 0}
    };

void getInputs() {
	__disable_irq();
	for (size_t i = 0; i < ANALOG_INPUTS; ++i) {
		analogInputs[i] = adc1Buffer[i];
	}
	bool buttonActive = false;
	uint8_t index = 0;

	// Read button matrix
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	for (int colIndex = 0; colIndex < 4; ++colIndex) {
		// Set the current column to output and low
		GPIO_InitStruct.Pin = colPins[colIndex];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(colPorts[colIndex], &GPIO_InitStruct);
		HAL_GPIO_WritePin(colPorts[colIndex], colPins[colIndex],
				GPIO_PIN_RESET);

		// Iterate through the rows
		for (int rowIndex = 0; rowIndex < 4; ++rowIndex) {
			GPIO_InitStruct.Pin = rowPins[rowIndex];
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(rowPorts[rowIndex], &GPIO_InitStruct);

			buttonActive = !HAL_GPIO_ReadPin(rowPorts[rowIndex],
					rowPins[rowIndex]);

			index = colIndex + rowIndex * 4;
			digitalInputs[index] = buttonActive;

			// Change LED state
			if (buttonActive) {
				ws2812_pixel(ledButtonMap[index], ledColors[index][0], ledColors[index][1], ledColors[index][2]);
			} else {
				ws2812_pixel(ledButtonMap[index], 0, 0, 0);
			}
			GPIO_InitStruct.Pull = GPIO_PULLDOWN;
			HAL_GPIO_Init(rowPorts[rowIndex], &GPIO_InitStruct);
		}

		// Disable the current column by setting it to input with pull-up
		GPIO_InitStruct.Pin = colPins[colIndex];
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(colPorts[colIndex], &GPIO_InitStruct);
	}

//	digitalInputs[0] = true;
//	digitalInputs[4] = true;
//	digitalInputs[7] = true;

	nextInputTime = time + INPUT_DELAY;

	// Prevent clock overflow
	if (nextInputTime > 10) {
		time = 0;
		nextInputTime = INPUT_DELAY;
	}

	__enable_irq();
}
