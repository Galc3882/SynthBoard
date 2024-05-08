#ifndef ADSRENVELOPE_HPP
#define ADSRENVELOPE_HPP

#include "Oscillator.hpp"

class ADSREnvelope {
public:
	ADSREnvelope(const bool (&noteStatesRef)[], const uint8_t &attackRef,
			const uint8_t &decayRef, const uint8_t &sustainRef,
			const uint8_t &releaseRef) :
			noteStatesRef(&noteStatesRef), attack(attackRef), decay(decayRef), sustain(
					sustainRef), release(releaseRef) {
	}

	inline void nextSample(uint16_t (&samples)[16]) {
		for (size_t i = 0; i < 16; ++i) {
			NoteState &ns = noteStates[i];
			const bool noteOn = (*noteStatesRef)[i];
			if (noteOn && ns.state == NoteState::Off) {
				ns.state = NoteState::Attack;
				ns.currentValue = 0;
				++activeCount;
			} else if (noteOn && ns.state == NoteState::Release) {
				ns.state = NoteState::Attack;
				ns.currentValue = 0;
			} else if (!noteOn && ns.state != NoteState::Off) {
				ns.state = NoteState::Release;
			}

			switch (ns.state) {
			case NoteState::Off:
				samples[i] = 0;
				break;
			case NoteState::Attack:
				if (attack <= 1) {
					ns.currentValue = 65280;
					ns.state = NoteState::Decay;
				} else {
					ns.currentValue +=
							(65280 / (attack * SAMPLE_RATE / factor));
					if (ns.currentValue >= 65280) {
						ns.currentValue = 65280;
						ns.state = NoteState::Decay;
					}
				}
				break;
			case NoteState::Decay:
				if ((decay) <= 1) {
					ns.currentValue = sustain * 256;
					ns.state = NoteState::Sustain;
				} else {
					ns.currentValue -= ((65280 - sustain * 256)
							/ (decay * SAMPLE_RATE / factor));
					if (ns.currentValue <= sustain * 255) {
						ns.currentValue = sustain * 255;
						ns.state = NoteState::Sustain;
					}
				}
				break;
			case NoteState::Sustain:
				// Hold the sustain level
				break;
			case NoteState::Release:
				if (release <= 1) {
					ns.currentValue = 0;
					ns.state = NoteState::Off;
					--activeCount;
				} else {
					ns.currentValue -= (sustain * 256
							/ (release * SAMPLE_RATE / factor));
					if (ns.currentValue <= 0) {
						ns.currentValue = 0;
						ns.state = NoteState::Off;
						--activeCount;
					}
				}
				break;
			}
			samples[i] = static_cast<uint16_t>(ns.currentValue);
		}
	}

	inline uint8_t getActiveNotesCount() const {
		return activeCount;
	}

private:
	struct NoteState {
		enum State {
			Off, Attack, Decay, Sustain, Release
		} state;
		int32_t currentValue;
		NoteState() :
				state(Off), currentValue(0) {
		}
	};

	NoteState noteStates[16];
	const bool (*noteStatesRef)[];
	const uint8_t &attack, &decay, &sustain, &release;
	uint8_t activeCount;
	const uint16_t factor = (10000 * 255) / NOTE_SCALING;
};

#endif // ADSRENVELOPE_HPP
