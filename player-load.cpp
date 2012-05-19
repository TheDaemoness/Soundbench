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
        reed->close();
        affectedlist->clear();
        affectedmet->setProgress(0);

        if(!reed->readerOpen(fname))
            return false; //Failed.

        if(reed->getFileType() == midi::SingleTrack) {
            bool a = loadTrack(0);
            return a;
        }
        else {
            affectedmet->setProgress(100);
            for(uint16_t i = (reed->getFileType() == midi::MultiTrack?1:0), e = reed->getTrackCount(); i < e; ++i) {
                affectedlist->addItem(reed->getTrackName(i).c_str());
                affectedmet->setProgress(100 + 800*(i+1)/e);
            }
            bool a;
            if (reed->getFileType() == midi::MultiTrack)
                a = loadTrack(1);
            else
                a = loadTrack(0);
            affectedmet->startMeter();
            return a;
        }
    }

    bool Player::loadTrack(uint16_t track) {
        std::cerr << "Parsing track #" << track+1 << " of the file...\n";
        if (reed->getFileType() == midi::MultiTrack)
            ++track;
        reed->readFrom(track);
        midi::MidiFileItem miditem;
        midi::MIDIEventNode* chiter = first;
        first->chainDestroy();

        while (true) {
            bool probl = false;
            miditem = reed->read();
            if (miditem.evtype == midi::EndOfTrack) {
                std::cerr << "Finished parsing.\n";
                break;
            }
            if (miditem.evtype == midi::NoDataRead) {
                std::cerr << "Failed to parse the file.\n";
                break;
            }

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
                case midi::SustenutoPedal:
                    chiter->attachNext(new midi::SustenutoEventNode(miditem.params.second, miditem.delay));
                    break;
                default:
                    std::cerr << "Request for unimplemented controller #" << static_cast<uint16_t>(miditem.params.first) << " made. Ignoring.\n";
                    probl = true;
                    break;
                }
                break;
            case midi::ProgramChange:
            case midi::ChannelPressure:
                std::cerr << "Warning: Events of type " << static_cast<uint16_t>(miditem.evtype) << " are ignored.\n";
                probl = true;
                break;
            case midi::Aftertouch:
                probl = true;
                break;
            case midi::SystemEvent:
                switch (miditem.meta) {
                default:
                    std::cerr << "An unimplemented meta event of type " << static_cast<uint16_t>(miditem.meta) << " was requested.\n";
                    probl = true;
                    break;
                }
                break;
            default:
                std::cerr << "Request for an unknown event of type " << static_cast<uint16_t>(miditem.evtype) << " made. Ignorning.\n";
                probl = true;
                break;
            }
            if (!probl)
                chiter = chiter->returnNext();
        }
        return true;
    }
}
