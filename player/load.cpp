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

    bool Player::loadTrack(uint16_t track) {
        if(!reed->isOpen())
            return false;
        std::cerr << "Loading track #" << track << "...\n";
        if (reed->getFileType() == midi::MultiTrack)
            ++track;
        if (!reed->readFrom(track)) {
            std::cerr << "Error switching tracks.\n";
            return false;
        }
        midi::MidiEvent miditem;
        midi::nodes::MIDIEventNode* chiter = first;
        first->chainDestroy();

        while (true) {
            miditem = reed->read();
            if (miditem.evtype == midi::NoData) {
                std::cerr << "Failed to parse the track.\n";
                break;
            }
            chiter->attachNext(midi::nodes::MIDIEventNode::makeNode(miditem));
            if (miditem.evtype == midi::EndOfTrack) {
                std::cerr << "Finished loading the track.\n";
                break;
            }
            chiter = chiter->returnNext();
        }
        return true;
    }
}
