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
#include <deque>

#include "errorpopup.h"
#include "warningpopup.h"
#include "common.h"

namespace sb {
    namespace midi {

        enum MidiControllers {
            DataSlider = 0x06, //Implement in v. 0.6.0.
            Volume = 0x07,
            Pan = 0x0A,
            ADSR_A = 0x10, //Implement in v. 0.8.0.
            ADSR_D = 0x11, //Implement in v. 0.8.0.
            ADSR_S = 0x12, //Implement in v. 0.8.0.
            ADSR_R = 0x13, //Implement in v. 0.8.0.
            HoldPedal = 0x40,
            SustPedal = 0x42,
            SoftPedal = 0x43, //Implement in v. 0.8.0.
            LegatoPedal = 0x44, //Implement in v. 0.8.0.
            ExtendRelease = 0x45, //Implement in v. 0.8.0.
            DataInc = 0x60, //Implement in v. 0.6.0.
            DataDec = 0x61, //Implement in v. 0.6.0.
            UnregParamNumLSB = 0x62, //Implement in v. 0.6.0.
            UnregParamNumMSB = 0x63, //Implement in v. 0.6.0.
            RegedParamNumLSB = 0x64, //Implement in v. 0.7.0.
            RegedParamNumMSB = 0x65 //Implement in v. 0.7.0.
        };
        enum MidiFileEvents {
            Failed = 0x0, //Not used in MIDI files; for internal use.
            EndOfTrack = 0x1, //Not used in MIDI files; for internal use.
            NoteOn = 0x8,
            NoteOff = 0x9,
            Aftertouch = 0xA,
            Controller = 0xB,
            ProgramChange = 0xC,
            ChannelPressure = 0xD,
            PitchBend = 0xE,
            Meta = 0xF
        };
        enum MidiMetaEvents {
            MetaEndOfTrack = 0x2F
        };

        struct MidiFileItem {
            bool read;
            midi::MidiFileEvents evtype;
            uint8_t chan;
            std::pair<unsigned char, unsigned char> params;
            uint8_t meta;
            std::string meta_data;
            uint32_t delay;
            bool operator==(std::string str) {
                return str==meta_data;
            }
            bool operator==(midi::MidiFileEvents ev) {
                return evtype==ev;
            }
        };

        class MidiFIO
        {
        public:
            MidiFIO() {
                writing = false;
                res_is_fps = false;
                res = 0;
                tracklen = 0;
                trackpos = 0;
            }
            ~MidiFIO() {
                river.close();
            }

            bool open(std::string, std::string mode = "r");
            void write(MidiFileItem);
            void readfrom(uint16_t); //Changes from which track read() reads.
            std::string checkTrackTitle(uint16_t); //Checks for a track title meta event.
            std::string getTrackName(uint16_t); //Checks the specified track for a meta-event immediately after the header giving the track's name.
            MidiFileItem read();
            bool close();

            inline uint16_t getTrackCount() {
                return filetype?tracks.size():1;
            }

        private:
            char filetype;
            std::vector<size_t> tracks; //Stores the read pointer offset to read from each track, starting with the Mtrk.
            std::fstream river;
            bool writing;
            bool res_is_fps;
            uint16_t res; //The tick time stored in here.
            size_t tracklen, trackpos;
        };
    }
}

#endif // MIDIREADER_H
