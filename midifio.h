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

#ifndef MIDIREADER_H
#define MIDIREADER_H

#include <fstream>

#ifdef IS_SOUNDBENCH
#include "errorpopup.h"
#include "warningpopup.h"
#endif

#include "common.h"
#include "midienums.h"

namespace sb {
    namespace midi {

        struct MidiFileItem {
            bool read;
            midi::MidiEvents evtype;
            uint8_t chan;
            std::pair<unsigned char, unsigned char> params;
            uint8_t meta;
            std::string meta_data;
            uint32_t delay;
            bool operator==(std::string str) {
                return str==meta_data;
            }
            bool operator==(midi::MidiEvents ev) {
                return evtype==ev;
            }
        };

        class MidiFIO
        {
        public:
            MidiFIO() {
                returnitem.evtype = midi::NoDataRead;
                writing = false;
                tracklen = 0;
                eot_reached = false;
            }
            ~MidiFIO() {
                river.close();
            }

#ifndef IS_SOUNDBENCH
            bool readerOpen(std::string, std::ostream& error_stream = std::cerr);
#else
            bool readerOpen(std::string);
#endif
            bool readFrom(uint16_t); //Changes from which track read() reads.
            std::string getTrackName(uint16_t); //Checks the specified track for a meta-event immediately after the header giving the track's name.
            MidiFileItem read();
            bool close();

            inline uint16_t getTrackCount() {
                return (filetype?tracks.size():1);
            }
            inline MidiFileTypes getFileType() {
                return static_cast<MidiFileTypes>(filetype);
            }

        private:
            char filetype;
            std::vector<size_t> tracks; //Stores the read pointer offset to read from each track, starting with the Mtrk.
            std::fstream river;
            bool writing, eot_reached, res_is_fps;
            union {
                struct {
                    uint16_t ticks_per_beat;
                    uint32_t microsecs_per_beat;
                } beats;
                struct {
                    uint8_t fps;
                    uint8_t ticks_per_frame;
                } frames;
            } res;
            size_t tracklen;
            MidiFileItem returnitem;
        };
    }
}

#endif // MIDIREADER_H
