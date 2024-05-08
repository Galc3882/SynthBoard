#include "event_loop.hpp"
#include "ws2812-spi.hpp" // https://www.newinnovations.nl/post/controlling-ws2812-and-ws2812b-using-only-stm32-spi/
#include "AUDIO.h" // https://controllerstech.com/waveplayer-using-stm32-discovery/#info_box

Synthesizer synth;
Oscillator oscillator(synth, digitalInputs, analogInputs[1], analogInputs[7]);
ADSREnvelope envelope(digitalInputs, analogInputs[2], analogInputs[3],
		analogInputs[4], analogInputs[5]);
LowPassFilter filter(analogInputs[6]);

// Main C++ event loop to run application
void EventLoopCpp() {
	// Initialize LEDs
	ws2812_init();

	// Initialize inputs
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc1Buffer, ADC1_BUFFER_SIZE);
	getInputs();

	fillAudioBuffer();  // Initially fill the whole buffer
	fillAudioBuffer();  // Fill the second half

	// Initialize audio output
	if (AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE,
			(uint8_t) analogInputs[0] / 2.55,
			SAMPLE_RATE) != AUDIO_OK) {
		// TODO: Error handling //HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin_Err, GPIO_PIN_SET);
	}

	// Start audio playback. Note that the size is in bytes, hence the multiplication by 2 (16-bit samples)
	AUDIO_OUT_Play((uint16_t*) &audioBuffer[0],
	AUDIO_BUFFER_SIZE * 2 * sizeof(uint16_t));

	// Main loop
	while (1) {
		// Handle inputs if delay passed
		if (nextInputTime <= time) {
			getInputs();
			// Set volume
			AUDIO_OUT_SetVolume((uint8_t) analogInputs[0] / 2.55);

			// Set waveform
			oscillator.determineWaveform(analogInputs[1]);

			// Set filter frequency
			filter.update(analogInputs[6]);

			// Set harmonics
			oscillator.setHarmonics(analogInputs[7]);
		}
	}
}

// Define all C function calls from main.c below
extern "C" {
void EventLoopC() {
	EventLoopCpp();
}
}
