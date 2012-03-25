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
#include "warningpopup.h"

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
        MidiFileEvents evtype;
        unsigned char chan;
        std::pair<unsigned char, unsigned char> params;
        std::string asciitext;
    };

    class MidiFIO
    {
    public:
        MidiFIO() {
            writing = false;
        }
        ~MidiFIO();
        bool open(std::string file, std::string mode = "r") {
            if (file.substr(file.size()-5,4) != ".mid") {
                WarningPopup* extensionwarning = new WarningPopup;
                extensionwarning->setWarningText("Wrong File Extension");
                extensionwarning->setInfoText(std::string(file) + " may not be a midi (.mid) file.\n\nBy ignoring this warning, the file will be read anyway.\n\nOtherwise, the import will be canceled.\n");
                extensionwarning->exec();
                bool goon = extensionwarning->returnContinue();
                delete extensionwarning;
                if (!goon)
                    return false;
            }
            if (mode == "r")
                river.open(file,std::ios_base::in|std::ios_base::binary);
            else if (mode == "w")
                river.open(file,std::ios_base::out|std::ios_base::trunc|std::ios_base::binary);
            return river.is_open();
        }

        void write(MidiFileItem);
        MidiFileItem read();

        void close() {
            if(river.is_open()) {

                river.close();
            }
            charisma.clear();
        }

        uint32_t timedata;
    private:
        std::vector<std::deque<unsigned char>> charisma; //A buffer for tracks from the file.
        std::fstream river;
        bool writing;
    };

}

#endif // MIDIREADER_H
