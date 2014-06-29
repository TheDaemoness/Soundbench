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

#include "soundbench.h"
#include "synth/synthsilent.h"

size_t sb::global_srate;

sb::Synth* sb::midi::nodes::MIDIEventNode::synref;

namespace sb {
	Soundbench::Soundbench() {
		global_srate = SAMPLING_RATES[1];

		syn = new sb::SynthSilent;
		em = new sb::Emitter(syn);
		plai = new sb::Player(syn);
	}

	Soundbench::~Soundbench() {
		delete plai;
		delete em;
		delete syn;
	}
}
