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

#include "midifio.h"

namespace sb {
    namespace midi {

        MidiEvent MidiFIO::read() {
            if (writing || !river.is_open())
                return returnitem;
            if (eot_reached) {
                returnitem.evtype = midi::EndOfTrack;
                return returnitem;
            }
            if (river.eof()) {
                std::cerr << "Reading ended prematurely (no End-Of-Track Meta Event?)\n";
                returnitem.evtype = midi::EndOfTrack;
                return returnitem;
            }

            //Deal with the delay.
            uint32_t raw_delay = 0;
            uint8_t byte;
            for(uint8_t i = 0; i < 4; ++i) {
                raw_delay <<= 7; //Assure space for the next seven bits (1 byte - the next-byte-exists bit).
                byte = river.get();
                raw_delay |= byte & ~Bit1; //Everything excpet the next-byte-exists bit.
                if (!(byte & Bit1)) //The next-byte-exists bit isn't set? Stop looping!
                    break;
            }

            returnitem.delay = factor*raw_delay;

            byte = river.get();
            if(byte & Bit1) {
                returnitem.evtype = static_cast<midi::MidiEvents>(byte >> 4); //Shift out the channel bits.
                returnitem.chan = byte & (Bit5 | Bit6 | Bit7 | Bit8); //Mask out all but the last four bits.

                if (returnitem.evtype != midi::SystemEvent) {
                    returnitem.params.first = river.get();
                    if (returnitem.evtype != ProgramChange && returnitem.evtype != ChannelPressure)
                        returnitem.params.second = river.get();
                }
            }
            else { //Running status.
                returnitem.params.first = byte;
                if (returnitem.evtype != ProgramChange && returnitem.evtype != ChannelPressure)
                    returnitem.params.second = river.get();
            }

            if (returnitem.evtype == midi::SystemEvent) {
                returnitem.meta_data.clear();
                //TODO: Sysex events?
                returnitem.meta = river.get();
                uint32_t evlen = 0;
                for(uint8_t i = 0; i < 4; ++i) { //See the previous loop that handles variable length data fields for comments.
                    evlen <<= 7;
                    byte = river.get();
                    evlen |= byte & ~Bit1;
                    if (!(byte & Bit1))
                        break;
                }
                for(uint32_t i = 0; i < evlen; ++i)
                    returnitem.meta_data.push_back(river.get());

                if (returnitem.meta == MetaTempo && ticks_per_beat != 0) {
                    factor = 0;
                    for(int i = 0; i < 3; i++) {
                        factor <<= 8;
                        factor += returnitem.meta_data[i];
                    }
                    factor /= ticks_per_beat;
                }
                if (returnitem.meta == MetaEndOfTrack) {
                    eot_reached = true;
                    returnitem.evtype = EndOfTrack;
                }
            }
            return returnitem;
        }
    }
}
