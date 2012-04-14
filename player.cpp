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
    bool Player::loadFile(std::string fname) {
        if(!reed->open(fname))
            return false; //Failed.
        reed->readfrom(0);
        midi::MidiFileItem miditem;
        midi::MIDIEventNode* chiter = first;

        while (true) {
            miditem = reed->read();
            if (miditem == midi::EndOfTrack || miditem == midi::Failed)
                break;

            switch(miditem.evtype) {
            case midi::NoteOn:
                chiter->attachNext(new midi::NoteOnEventNode(miditem.params.first-69, /*69 is the MIDI number for A4*/
                                                             static_cast<sbSample>(miditem.params.second)/127,
                                                             miditem.delay));
                break;
            case midi::NoteOff:
                chiter->attachNext(new midi::NoteOffEventNode(miditem.params.first-69, miditem.delay));
                break;

            case midi::Controller:
                switch (miditem.params.first) {
                case midi::HoldPedal:
                    chiter->attachNext(new midi::HoldPedalEventNode(miditem.params.second, miditem.delay));
                    break;
                case midi::SustPedal:
                    chiter->attachNext(new midi::SustenuoEventNode(miditem.params.second, miditem.delay));
                    break;
                default:
                    std::cerr << "Request for unimplemented controller #" << miditem.params.first << " made. Ignoring.\n";
                }
                break;

            default:
                std::cerr << "Request for unknown event type #" << miditem.params.first << " made. Ignoring.\n";
            break;
            }

            chiter = chiter->returnNext();
        }
        return true;
    }
}
