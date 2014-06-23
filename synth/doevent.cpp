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

#include "midifio/midievents.h"
#include "synth/synth.h"

namespace sb {
	namespace midi {
		bool doEvent(Synth* syn, MidiEvent miditem) {
			switch(miditem.evtype) {
			case NoteOn:
				if (miditem.params.second != 0)
					syn->noteOn(miditem.params.first-69,static_cast<float>(miditem.params.second)/127);
				else
					syn->noteOff(miditem.params.first-69);
				return true;
			case NoteOff:
				syn->noteOff(miditem.params.first-69);
				return true;
			case Controller:
				switch (miditem.params.first) {
				case ctrl::HoldPedal:
					syn->pedal(Hold,miditem.params.second >= 64);
					return true;
				case ctrl::SustenutoPedal:
					syn->pedal(Sustenuto,miditem.params.second >= 64);
					return true;
				default:
					return false;
				}
			default:
				return false;
			}
		}
	}
}
