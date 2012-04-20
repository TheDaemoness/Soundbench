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

#include "midifile.h"

namespace sb {
    namespace midi {

        MidiFileItem MidiFIO::read() {
            MidiFileItem returnitem;
            returnitem.evtype = midi::Failed;
            if (writing || !river.is_open())
                return returnitem;
            if (trackpos >= tracklen) {
                returnitem.evtype = midi::EndOfTrack;
                return returnitem;
            }
            if (river.eof()) {
                std::cerr << "Reading ended prematurely. Perhaps the track header is corrupted.\n";
                return returnitem;
            }
            returnitem.delay = 0;
            uint8_t byte;
            for(uint8_t i = 0; i < 4; ++i) {
                returnitem.delay <<= 7; //Assure space for the next seven bits (1 byte - the next-byte-exists bit).
                byte = river.get();
                returnitem.delay |= byte & 127; //Everything excpet the next-byte-exists bit.
                ++trackpos;
                if (!(byte & 128)) //The next-byte-exists bit isn't set? Stop looping!
                    break;
            }
            byte = river.get();
            ++trackpos;
            returnitem.evtype = static_cast<midi::MidiFileEvents>(byte >> 4); //Shift out the channel bits.
            returnitem.chan = (byte << 4) >> 4; //Shift out the event type bits.
            if (returnitem.evtype != midi::Meta) {
                returnitem.params.first = river.get();
                returnitem.params.second = river.get();
                trackpos += 2;
            }
            else {
                //TODO: Sysex events?
                returnitem.meta = river.get();
                uint32_t evlen = 0;
                for(uint8_t i = 0; i < 4; ++i) { //See the previous loop that handles variable length data fields for comments.
                    evlen <<= 7;
                    byte = river.get();
                    evlen |= byte & 127;
                    if (!(byte & 128))
                        break;
                }
                trackpos += evlen;
                for(uint32_t i = 0; i < evlen; ++i) {
                    returnitem.meta_data.push_back(river.get());
                }
            }
            return returnitem;
        }
    }
}
