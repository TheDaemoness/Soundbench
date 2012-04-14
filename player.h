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


#ifndef PLAYER_H
#define PLAYER_H

#include "midi.h"
#include "midichain.h"
#include "midifile.h"
#include <deque>

namespace sb {

    class Player
    {
    private:
        midi::MidiFIO* reed;
        midi::PlayerStartNode* first;
        std::deque<midi::MIDIEventNode> nodes;
        unsigned char tempo;
        time_t midiclocklen;
    public:
        explicit Player(Synth* syn) {
            reed = NULL;
            first = new midi::PlayerStartNode(syn);
            tempo = 108;
            /*
              microseconds/beat = 1 / (beats/minute * minutes/second * seconds/microsecond)
              */
            midiclocklen = static_cast<time_t>(1.0 / (static_cast<double>(tempo)/(0.000000017)));
            first->accessMIDIClockLen(&midiclocklen);
        }
        bool loadFile(std::string);
        ~Player() {
            delete first;
        }
        bool loadTrack(uint16_t track);
        inline void play() {
            first->run();
        }
        inline void stop() {
            first->stop();
        }
        bool writeFile(std::string);
    };
}

#endif // PLAYER_H
