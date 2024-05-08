#include "AudioBuffer.hpp"
#include <numeric>

uint16_t audioBuffer[AUDIO_BUFFER_SIZE * 2];
bool bufferHalf = false;
float time = 0;

// Fills one half of the audio buffer with synthesized audio samples
inline void fillAudioBuffer() {
	uint16_t *bufferPtr =
			bufferHalf ? &audioBuffer[AUDIO_BUFFER_SIZE] : &audioBuffer[0];

	int16_t oscillatorSamples[16] = { 0 };
	uint16_t envelopes[16] = { 0 };
	int64_t sum;
	int16_t envelopeSample;
	uint16_t filteredSample;
	uint8_t activeNotes;

	for (size_t i = 0; i < AUDIO_BUFFER_SIZE; ++i) {
		oscillator.nextSample(oscillatorSamples);
		envelope.nextSample(envelopes);

		activeNotes = envelope.getActiveNotesCount();
		if (activeNotes == 0) {
			std::fill_n(bufferPtr, AUDIO_BUFFER_SIZE, 0);
			bufferHalf = !bufferHalf;
			return;
		}

		sum = 0;
		for (size_t j = 0; j < 16; ++j) {
			sum += oscillatorSamples[j] * envelopes[j];
		}

		// Normalize by max size of int16 and max number of notes
		envelopeSample = static_cast<int16_t>(sum >> (16 + 4 - 1));
		filteredSample = static_cast<uint16_t>(filter.process(envelopeSample));
		bufferPtr[i] = filteredSample;
		bufferPtr[++i] = filteredSample;
	}
	bufferHalf = !bufferHalf;
}

// Callback when half of the audio buffer is played
extern "C" void AUDIO_OUT_HalfTransfer_CallBack(void) {
	time += 10000.0f * AUDIO_BUFFER_SIZE / (SAMPLE_RATE * NOTE_SCALING);

	// Stop interrupt nesting if no user input for some time
	if (time > nextInputTime + 1) {
		__disable_irq();
		return;
	}

	bufferHalf = false;  // First half of the buffer will be filled next
	fillAudioBuffer();  // Fill the first half of the buffer
}

// Callback when the full audio buffer is played
extern "C" void AUDIO_OUT_TransferComplete_CallBack(void) {
	time += 10000.0f * AUDIO_BUFFER_SIZE / (SAMPLE_RATE * NOTE_SCALING);

	// Stop interrupt nesting if no user input for some time
	if (time > nextInputTime + 1) {
		__disable_irq();
		return;
	}

	bufferHalf = true;  // Second half of the buffer will be filled next
	fillAudioBuffer();  // Fill the second half of the buffer
}
