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

#include "player.h"

namespace sb {
	void Player::initfrontend(Synth* syn) {
#ifdef NO_MIDIFRONTEND
		std::cerr << "Soundbench was compiled without any MIDI frontends.\n";
#else
		std::cerr << "Determining which MIDI frontends will initialize.\n";
		supported_apis[RTMIDI_I] = RtMidiFrontend::instantiable();
		supported_apis[JACK_I] = JackMidiFrontend::instantiable();
#endif

		if (supported_apis[RTMIDI_I]) {
			std::cerr << "Initializing an RtMidi frontend...\n";
			midin = new RtMidiFrontend(syn);
			if (!midin->isReady()) {
				std::cerr << "Failed to initialize RtMidi frontend.\n";
				delete midin;
			}
			else {
				std::cerr << "RtMidi frontend initialized.\n";
				return;
			}
		}
		std::cerr << "No MIDI frontends could be initialized on this system.\n";
		midin = nullptr;
	}
}
