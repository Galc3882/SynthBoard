#ifndef LOWPASSFILTER_HPP
#define LOWPASSFILTER_HPP

#include "Oscillator.hpp"

class LowPassFilter {
public:
	LowPassFilter(uint8_t cutoffFrequency) :
			cutoffFrequency(cutoffFrequency > 10 ? cutoffFrequency : 10), previousOutput(
					0) {
	}

	inline int16_t process(int16_t input) {
		int16_t output = (cutoffFrequency * input
				+ (255 - cutoffFrequency) * previousOutput) / 255;
		previousOutput = output;
		return output;
	}

	void update(uint8_t newCutoffFrequency) {
		if (newCutoffFrequency > 10) {
			cutoffFrequency = newCutoffFrequency;
		} else {
			cutoffFrequency = 10;
		}
	}

private:
	uint8_t cutoffFrequency;
	int16_t previousOutput;
};

#endif // LOWPASSFILTER_HPP
