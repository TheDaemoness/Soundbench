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

	Copyright 2012  Amaya S.
*/

#include "midifio.h"
#include "util/fileparsers.h"

namespace sb {
	namespace midi {

		MidiEvent MidiFIO::read() {
			if (writing || !river.is_open())
				return returnitem;
			if (eot_reached) {
				returnitem.evtype = midi::EndOfTrack;
				return returnitem;
			}
			if (river.eof()) {
				std::cerr << "Reading ended prematurely (no End-Of-Track Meta Event?)\n";
				returnitem.evtype = midi::EndOfTrack;
				return returnitem;
			}

			//Deal with the delay.
			uint32_t raw_delay = vliParse<4>(river);
			returnitem.delay = factor*raw_delay;

			uint8_t byte = river.get();
			if(byte & Bit1) {
				returnitem.evtype = static_cast<MidiEvents>(byte & Nibble1); //Mask out the channel bits.
				returnitem.chan = byte & Nibble2; //Mask out the event type bits.

				if (returnitem.evtype != midi::System) {
					returnitem.params.first = river.get();
					if (returnitem.evtype != ProgramChange && returnitem.evtype != ChannelPressure)
						returnitem.params.second = river.get();
				}
			}
			else { //Running status.
				returnitem.params.first = byte;
				if (returnitem.evtype != ProgramChange && returnitem.evtype != ChannelPressure)
					returnitem.params.second = river.get();
			}

			if (returnitem.evtype == midi::System) {
				returnitem.meta_data.clear();
				if (returnitem.chan != midi::MetaEvent)
					return returnitem; //Filter sysex events;

				returnitem.meta = river.get();
				uint32_t evlen = 0;
				for(uint8_t i = 0; i < 4; ++i) { //See the previous loop that handles variable length data fields for comments.
					evlen <<= 7;
					byte = river.get();
					evlen |= byte & ~Bit1;
					if (!(byte & Bit1))
						break;
				}
				for(uint32_t i = 0; i < evlen; ++i)
					returnitem.meta_data.push_back(river.get());

				if (returnitem.meta == MetaTempo && ticks_per_beat != 0) {
					factor = 0;
					for(int i = 0; i < 3; i++) {
						factor <<= 8;
						factor += returnitem.meta_data[i];
					}
					factor /= ticks_per_beat;
				}
				if (returnitem.meta == MetaEndOfTrack) {
					eot_reached = true;
					returnitem.evtype = EndOfTrack;
				}
			}
			return returnitem;
		}
	}
}
