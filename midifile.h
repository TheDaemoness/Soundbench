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

#include "sbutilities.h"
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
                writing = false;
                tracklen = 0;
            }
            ~MidiFIO() {
                river.close();
            }

            bool readerOpen(std::string, std::ostream& error_stream = std::cerr);
            bool writerOpen(std::string, std::ostream& error_stream = std::cerr);
            void write(MidiFileItem);
            void readFrom(uint16_t); //Changes from which track read() reads.
            std::string checkTrackTitle(uint16_t); //Checks for a track title meta event.
            std::string getTrackName(uint16_t); //Checks the specified track for a meta-event immediately after the header giving the track's name.
            MidiFileItem read();
            bool close();

            inline uint16_t getTrackCount() {
                return (filetype?tracks.size():1);
            }

        private:
            char filetype;
            std::vector<size_t> tracks; //Stores the read pointer offset to read from each track, starting with the Mtrk.
            std::fstream river;
            bool writing, eot_reached, res_is_fps;
            union {
                struct {
                    uint16_t ticks_per_beat;
                    uint16_t tempo;
                } beats;
                struct {
                    uint8_t fps;
                    uint8_t ticks_per_frame;
                } frames;
            } res;
            size_t tracklen;
        };
    }
}

#endif // MIDIREADER_H
