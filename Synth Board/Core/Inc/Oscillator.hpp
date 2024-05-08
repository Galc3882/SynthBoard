#ifndef OSCILLATOR_HPP
#define OSCILLATOR_HPP

#include <stdint.h>
#include "Synthesizer.hpp"

#define SAMPLE_RATE 31960
#define NOTE_SCALING 16250*2 // 4 sig figs

enum NoteFrequencies {
	C4 = 261,  // Middle C
	Cs4 = 277, // C#4/Db4
	D4 = 293,  // D
	Ds4 = 311, // D#4/Eb4
	E4 = 329,  // E
	F4 = 349,  // F
	Fs4 = 369, // F#4/Gb4
	G4 = 391,  // G
	Gs4 = 415, // G#4/Ab4
	A4 = 440,  // A (standard tuning)
	As4 = 466, // A#4/Bb4
	B4 = 493,   // B
	C5 = 523,	// High C
	Cs5 = 554,	// C#5/Db5
	D5 = 587,	// D5
	Ds5 = 622,	// D#5/Eb5
	E5 = 659	// E5
};

const NoteFrequencies noteFrequencies[] = { Ds4, D4, Cs4, C4, B4, As4, A4, Gs4,
		G4, Fs4, F4, E4, Ds5, E5, Cs5, C5 }; // remapped because buttons were wired out of order

class Oscillator {
public:
	enum Waveform {
		Sine, Square, Sawtooth, Triangle
	};

	Oscillator(Synthesizer &synth, const bool (&noteStatesRef)[], uint8_t waveformControl,
			uint8_t numHarmonics) :
			synth(synth), noteStatesRef(&noteStatesRef), currentWaveform(static_cast<Waveform>(waveformControl >> 6)), harmonics(
					1 + (numHarmonics >> 5)) {
	}

	inline void nextSample(int16_t (&samples)[16]) {
		uint16_t frequency;
		for (size_t i = 0; i < 16; ++i) {
			if (!(*noteStatesRef)[i])
				continue;

			uint16_t &phase = phases[i];

			switch (currentWaveform) {
			case Sine:
				sample = synth.generateSineWave(phase, harmonics);
				break;
			case Square:
				sample = synth.generateSquareWave(phase, harmonics);
				break;
			case Sawtooth:
				sample = synth.generateSawtoothWave(phase, harmonics);
				break;
			case Triangle:
				sample = synth.generateTriangleWave(phase, harmonics);
				break;
			}

			frequency = noteFrequencies[i] * NOTE_SCALING / 10000;
			phase += frequency * static_cast<int32_t>(65535) / SAMPLE_RATE;
			samples[i] = sample;
		}
	}

	void determineWaveform(uint8_t waveformControl) {
		currentWaveform = static_cast<Waveform>(waveformControl >> 6);
	}

	void setHarmonics(uint8_t numHarmonics) {
		harmonics = 1 + (numHarmonics >> 5);
	}
private:
	Synthesizer &synth;
	const bool (*noteStatesRef)[];
	Waveform currentWaveform;
	uint16_t phases[16] = { 0 };
	int16_t sample;
	uint8_t harmonics;
};

#endif // OSCILLATOR_HPP
