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
    bool Player::loadFile() {
        reed->close();
        affectedlist->clear();

        if(!reed->readerOpen(fi)) {
            std::cerr << "Aborted reading.\n";
            return false;
        }

        if(reed->getFileType() == midi::SingleTrack)
            return true;
        else {
            for(uint16_t i = (reed->getFileType() == midi::MultiTrack?1:0), e = reed->getTrackCount(); i < e; ++i)
                affectedlist->addItem(reed->getTrackName(i).c_str());
            if (reed->getFileType() == midi::MultiTrack) {
                if(!loadTrack(1))
                    return false;
            }
            else {
                if(!loadTrack(0))
                    return false;
            }
            affectedlist->setCurrentRow(0);
            return true;
        }
    }

    bool Player::loadTrack(uint16_t track) {
        std::cerr << "Loading track #" << track << "...\n";
        if (reed->getFileType() == midi::MultiTrack)
            ++track;
        if (!reed->readFrom(track)) {
            std::cerr << "Error switching tracks.\n";
            return false;
        }
        midi::MidiFileItem miditem;
        midi::MIDIEventNode* chiter = first;
        first->chainDestroy();

        while (true) {
            miditem = reed->read();
            if (miditem.evtype == midi::EndOfTrack) {
                std::cerr << "Finished loading the track.\n";
                break;
            }
            if (miditem.evtype == midi::NoDataRead) {
                std::cerr << "Failed to parse the track.\n";
                break;
            }
            switch(miditem.evtype) {
            case midi::NoteOn:
                if (miditem.params.second != 0)
                chiter->attachNext(new midi::NoteOnEventNode(miditem.params.first-69, /*69 is the MIDI number for A4*/
                                                             static_cast<SbSample>(miditem.params.second)/127,
                                                             miditem.delay));
                else
                    chiter->attachNext(new midi::NoteOffEventNode(miditem.params.first-69, miditem.delay));
                break;
            case midi::NoteOff:
                chiter->attachNext(new midi::NoteOffEventNode(miditem.params.first-69, miditem.delay));
                break;

            case midi::Controller:
                switch (miditem.params.first) {
                case midi::HoldPedal:
                    chiter->attachNext(new midi::HoldPedalEventNode(miditem.params.second, miditem.delay));
                    break;
                case midi::SustenutoPedal:
                    chiter->attachNext(new midi::SustenutoEventNode(miditem.params.second, miditem.delay));
                    break;
                default:
                    std::cerr << "Request for unimplemented controller #" << static_cast<uint16_t>(miditem.params.first) << " made. Ignoring.\n";
                    chiter->attachNext(new midi::DelayNode(miditem.delay));
                    break;
                }
                break;
            case midi::ProgramChange:
            case midi::ChannelPressure:
            case midi::PitchBend:
                std::cerr << "Warning: Events of type " << static_cast<uint16_t>(miditem.evtype) << " are ignored.\n";
                chiter->attachNext(new midi::DelayNode(miditem.delay));
                break;
            case midi::Aftertouch:
                chiter->attachNext(new midi::DelayNode(miditem.delay));
                break;
            case midi::SystemEvent:
                switch (miditem.meta) {
                case midi::MetaTempo:
                case midi::MetaTrackName:
                    chiter->attachNext(new midi::DelayNode(miditem.delay));
                    break;
                default:
                    std::cerr << "An unimplemented meta event of type " << static_cast<uint16_t>(miditem.meta) << " was requested.\n";
                    chiter->attachNext(new midi::DelayNode(miditem.delay));
                    break;
                }
                break;
            default:
                std::__throw_logic_error("The MIDI file reader ignored running status. If you see this message while running an official binary, then please report it as a bug. It shouldn't take too long to fix.");
                break;
            }
            chiter = chiter->returnNext();
        }
        isready = true;
        return true;
    }
}
