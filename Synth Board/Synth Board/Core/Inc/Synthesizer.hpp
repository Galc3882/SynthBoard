#ifndef SYNTHESIZER_HPP
#define SYNTHESIZER_HPP

#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Synthesizer {
public:
	Synthesizer() {
		initializeSineTable();
	}

	inline int16_t generateSineWave(uint16_t phase, uint8_t harmonics) {
		sample = 0;
		for (i = 1; i < harmonics + 1; ++i) {
			sample += sineTable[static_cast<uint16_t>(phase * i) >> 2] / i;
		}

		return static_cast<int16_t>(sample / 3);
	}

	inline int16_t generateSquareWave(uint16_t phase, uint8_t harmonics) {
		sample = 0;
		for (i = 1; i < harmonics + 1; ++i) {
			sample +=
					(static_cast<uint16_t>(phase * i) < 32767 ? 32767 : -32768)
							/ i;
		}

		return static_cast<int16_t>(sample / 3);
	}

	inline int16_t generateSawtoothWave(uint16_t phase, uint8_t harmonics) {
		sample = 0;
		for (i = 1; i < harmonics + 1; ++i) {
			sample += (static_cast<int16_t>(static_cast<uint16_t>(phase * i)
					- 32768)) / i;
		}

		return sample / 3;
	}

	inline int16_t generateTriangleWave(uint16_t phase, uint8_t harmonics) {
		sample = 0;
		for (i = 1; i < harmonics + 1; ++i) {
			scaledPhase = phase * i;
			if (scaledPhase < 16384) {
				sample += ((scaledPhase * 32767) >> 14) / i;
			} else if (scaledPhase < 49152) {
				sample += ((32767 - ((scaledPhase - 16384) * 32767 >> 14))) / i;
			} else {
				sample += ((((scaledPhase - 49152) * 32767) >> 14) - 32767) / i;
			}
		}

		return static_cast<int16_t>(sample / 3);
	}

private:
	static const uint16_t tableSize = 16385;
	int16_t sineTable[tableSize];
	int32_t sample;
	int8_t i;
	uint16_t scaledPhase;

	void initializeSineTable() {
		for (size_t i = 0; i < tableSize; ++i) {
			sineTable[i] = std::sin(2 * M_PI * i / tableSize) * 32767;
		}
	}
};

#endif // SYNTHESIZER_HPP
