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

#include "warningpopup.h"
#include "common.h"

namespace sb {

    enum MidiFileEvents {
        NoteOn = 0x8,
        NoteOff = 0x9,
        Aftertouch = 0xA,
        Controller = 0xB,
        PitchBend = 0xE,
        Meta = 0xF
        //Soundbench ignores all MIDI events written in files not listed here.
    };

    struct MidiFileItem {
        bool read;
        MidiFileEvents evtype;
        unsigned char chan;
        std::pair<unsigned char, unsigned char> params;
        std::string asciitext;
        bool operator==(std::string str) {
            return str==asciitext;
        }
        bool operator==(MidiFileEvents ev) {
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
        ~MidiFIO();
        bool open(std::string, std::string mode = "r");

        void write(MidiFileItem); //Somewhat of a misnormer. This writes to a buffer, not the open file.
        void readfrom(uint16_t); //Changes from which track read() reads.
        MidiFileItem read();
        bool close(); //Do not change to void! This function has to return whether a write of the buffer was successful.
    private:
        char filetype;
        std::vector<size_t> tracks; //Stores the read pointer offset to read from each track, starting with the Mtrk.
        std::fstream river;
        bool writing;
        bool res_is_fps;
        uint16_t res; //The tick time stored in
        size_t tracklen, trackpos; //Tracklen is used to indicate the value just after a relevant Mtrk. Trackpos is a current position.
    };

}

#endif // MIDIREADER_H
