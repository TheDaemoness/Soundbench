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

#include "synth.h"
#include "midienums.h"

#include <thread>

namespace sb {
    namespace midi {

        class PlayerStartNode;

        class MIDIEventNode {
        protected:
            friend class PlayerStartNode;

            MIDIEventNode* next;
            uint64_t beat;
            static time_t* clockmicrosecs;
            static Synth* synref;
        public:
            explicit MIDIEventNode(uint16_t beet = 0) {
                beat = beet;
                next = nullptr;
            }
            MIDIEventNode* nextDestroy() {
                return next->returnNext();
                delete next;
            }
           void chainDestroy() {
                if (next != nullptr) {
                    next->chainDestroy();
                    delete next;
                }
            }
            virtual ~MIDIEventNode() {
                chainDestroy();
            }


            void accessMIDIClockLen(time_t* microsecs) {
                clockmicrosecs = microsecs;
            }

            time_t accessMIDIClockLen() {
                return *clockmicrosecs;
            }

            uint64_t getDelay() {
                return beat;
            }
            MIDIEventNode* returnNext() {
                return next;
            }
            void attachNext(MIDIEventNode* nod = nullptr) {
                if (next != nullptr)
                    delete next;
                next = nod;
            }

            virtual void doevent() = 0;
        };



        class MIDIEventIterator {
            //WARNING: Objects of this type delete themselves.
            std::thread* tread;
            friend class PlayerStartNode;
            PlayerStartNode* star;
            explicit MIDIEventIterator(PlayerStartNode*);
        };

        class PlayerStartNode : public MIDIEventNode {
            MIDIEventIterator* it;
            bool running;
            time_t* midiclocklen;
        public:
            explicit PlayerStartNode(Synth* syn) {
                MIDIEventNode::synref = syn;
                it = nullptr;
                running = false;
            }

            ~PlayerStartNode() {
                if (it != nullptr)
                    delete it;
            }
            bool isRunning() {
                return running;
            }
            void doevent() {}
            void run() {
                if (it == nullptr)
                    it = new MIDIEventIterator(this);
                else {
                    delete it;
                    it = new MIDIEventIterator(this);
                }
                running = true;
            }
            void stop() {
                if (it != nullptr) {
                    running = false;
                    it = nullptr;
                }
            }
        };

        class NoteOnEventNode : public MIDIEventNode {
            int halfsteps;
            sbSample amp;
        public:
            explicit NoteOnEventNode(int notedist, sbSample ampe = 1.0, uint16_t beet = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(beet) {
                next = nexte;
                halfsteps = notedist;
                amp = ampe;
            }
            void doevent() {
                synref->noteOn(halfsteps,amp);
            }
        };

        class NoteOffEventNode : public MIDIEventNode {
            int halfsteps;
        public:
            explicit NoteOffEventNode(int notedist, uint16_t delai = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(delai) {
                next = nexte;
                halfsteps = notedist;
            }
            void doevent() {
                synref->noteOff(halfsteps);
            }
        };

        class HoldPedalEventNode : public MIDIEventNode {
            uint8_t pressure;
        public:
            explicit HoldPedalEventNode(uint8_t press, uint16_t delai = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(delai) {
                next = nexte;
                pressure = press;
            }
            void doevent() {
                if (pressure > 63)
                    synref->pedal('h',true);
                else
                    synref->pedal('h',false);
            }
        };

        class SustenutoEventNode : public MIDIEventNode {
            uint8_t pressure;
        public:
            explicit SustenutoEventNode(uint8_t press, uint16_t delai = 0, MIDIEventNode* nexte = nullptr) : MIDIEventNode(delai) {
                next = nexte;
                pressure = press;
            }
            void doevent() {
                if (pressure > 63)
                    synref->pedal('s',true);
                else
                    synref->pedal('s',false);
            }
        };
    }
}

#endif // MIDICHAIN_H
