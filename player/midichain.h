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

#ifndef MIDICHAIN_H
#define MIDICHAIN_H

#include "synth/synth.h"
#include "midifio/midievents.h"

#include <thread>

namespace sb {
	namespace midi {
		namespace nodes {
			class PlayerStartNode;

			class MIDIEventNode {
			protected:
				friend class PlayerStartNode;

				MIDIEventNode* next;
				uint64_t delae;
				static Synth* synref; //In main.cpp
			public:
				explicit MIDIEventNode(uint64_t tiim = 0) {
					delae = tiim;
					next = nullptr;
				}
				MIDIEventNode* detachNext() {
					MIDIEventNode* evnode = next;
					next = nullptr;
					return evnode;

				}
				void chainDestroy() {
					if (next != nullptr) {
						next->chainDestroy();
						delete next;
						next = nullptr;
					}
				}
				virtual ~MIDIEventNode() {
					chainDestroy();
				}

				uint32_t getDelay() {
					return delae;
				}
				MIDIEventNode* returnNext() {
					return next;
				}
				virtual void attachNext(MIDIEventNode* nod = nullptr) {
					if (next != nullptr)
						delete next;
					next = nod;
				}

				virtual void doEvent() = 0;
				virtual MidiEvent getEvent() = 0;
			};

			MIDIEventNode* makeNode(MidiEvent);

			class PlayerStartNode : public MIDIEventNode {
				bool running;
			public:
				explicit PlayerStartNode(Synth* syn) : MIDIEventNode(0) {
					running = false;
					MIDIEventNode::synref = syn;
				}
				void play() {};
				void stop() {};
				bool isRunning() {
					return running;
				}

				void doEvent() {}
				virtual MidiEvent getEvent() {
					MidiEvent ev;
					ev.evtype = midi::NoData;
					return ev;
				}
			};

			class DelayNode : public MIDIEventNode { //This is mostly a placeholder for unimplemented and unsupported events.
			public:
				explicit DelayNode(uint64_t taim = 0) : MIDIEventNode(taim) {}
				void doEvent() {}
				virtual MidiEvent getEvent() {
					MidiEvent ev;
					ev.evtype = midi::Dummy;
					ev.delay = delae;
					return ev;
				}
			};

			class ResetNode : public MIDIEventNode {
			public:
				explicit ResetNode(uint64_t taim = 0) : MIDIEventNode(taim) {}
				void doEvent() {
					synref->reset();
				}
				virtual MidiEvent getEvent() {
					MidiEvent ev;
					ev.evtype = midi::Dummy;
					ev.delay = delae;
					return ev;
				}
			};

			class EndNode : public MIDIEventNode {
			public:
				explicit EndNode(uint64_t taim = 0) : MIDIEventNode(taim) {}
				void doEvent() {
					synref->reset();
				}
				void attachNext(MIDIEventNode*) {}
				virtual MidiEvent getEvent() {
					MidiEvent ev;
					ev.evtype = midi::System;
					ev.delay = delae;
					ev.meta = midi::MetaEndOfTrack;
					return ev;
				}
			};

			class NoteOnEventNode : public MIDIEventNode {
				int halfsteps;
				SbSample amp;
			public:
				explicit NoteOnEventNode(int notedist, SbSample ampe = 1.0, uint64_t beet = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(beet) {
					next = nexte;
					halfsteps = notedist;
					amp = ampe;
				}
				void doEvent() {
					synref->noteOn(halfsteps,amp);
				}
				virtual MidiEvent getEvent() {
					MidiEvent ev;
					ev.evtype = midi::NoteOn;
					ev.delay = delae;
					ev.params.first = 69 + halfsteps;
					ev.params.second = 127*amp;
					return ev;
				}
			};

			class NoteOffEventNode : public MIDIEventNode {
				int halfsteps;
			public:
				explicit NoteOffEventNode(int notedist, uint64_t delai = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(delai) {
					next = nexte;
					halfsteps = notedist;
				}
				void doEvent() {
					synref->noteOff(halfsteps);
				}
				virtual MidiEvent getEvent() {
					MidiEvent ev;
					ev.evtype = midi::NoteOn;
					ev.delay = delae;
					ev.params.first = 69 + halfsteps;
					ev.params.second = 0;
					return ev;
				}
			};

			class HoldPedalEventNode : public MIDIEventNode {
				uint8_t pressure;
			public:
				explicit HoldPedalEventNode(uint8_t press, uint64_t delai = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(delai) {
					next = nexte;
					pressure = press;
				}
				void doEvent() {
					if (pressure > 63)
						synref->pedal(Hold,true);
					else
						synref->pedal(Hold,false);
				}
				virtual MidiEvent getEvent() {
					MidiEvent ev;
					ev.evtype = midi::Controller;
					ev.delay = delae;
					ev.params.first = midi::ctrl::HoldPedal;
					ev.params.second = pressure;
					return ev;
				}
			};

			class SustenutoEventNode : public MIDIEventNode {
				uint8_t pressure;
			public:
				explicit SustenutoEventNode(uint8_t press, uint64_t delai = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(delai) {
					next = nexte;
					pressure = press;
				}
				void doEvent() {
					if (pressure > 63)
						synref->pedal(Sustenuto,true);
					else
						synref->pedal(Sustenuto,false);
				}
				virtual MidiEvent getEvent() {
					MidiEvent ev;
					ev.evtype = midi::Controller;
					ev.delay = delae;
					ev.params.first = midi::ctrl::SustenutoPedal;
					ev.params.second = pressure;
					return ev;
				}
			};
		}
	}
}

#endif // MIDICHAIN_H
