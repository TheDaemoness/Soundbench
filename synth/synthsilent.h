/*
	This file is part of Soundbench.

	Soundbench is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Soundbench is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Soundbench.  If not, see <http://www.gnu.org/licenses/>.

	Copyright 2014  Amaya S.
*/

#include "synth.h"

#ifndef SYNTHSILENT_H
#define SYNTHSILENT_H

namespace sb {
	class SynthSilent : public Synth {
	public:
		SynthSilent() {}
		void noteOn(int halfsteps, SbSample amp) {}
		void noteOff(int halfsteps) {}
		void pedal(SupportedPedals which, bool val = true) {}
		void reset() {}
		void tick(SbSample* frame) {*frame = SbSample_ZERO;}
		uint8_t channel_count() {return 0;}

		void setPolyphony(uint8_t) {}
		void updateSamplingRate() {}

		void interleaved_block(SbSample* frames, size_t framecount) {
			for(size_t i = 0; i < framecount*sb::OUTPUT_CHANS; ++i)
				frames[i] = 0;
		}
		void uninterleaved_blocks(SbSample* lframes, SbSample* rframes, size_t framecount) {
			for(size_t i = 0; i < framecount; ++i) {
				lframes[i] = 0;
				rframes[i] = 0;
			}
		}
	};
}

#endif // SYNTHSILENT_H
