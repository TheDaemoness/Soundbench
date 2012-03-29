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

//NOTICE: This file is commented to make it easier for the reader to understand.

namespace sb {
    void MidiFIO::write(MidiFileItem writeitem) {

    }

    void MidiFIO::readfrom(uint16_t traque) {
        if(river.is_open() && !writing && traque < tracks.size())
            river.seekg(tracks[traque]+4); //Soundbench can ignore the 4 bytes with the Mtrk.
    }

    MidiFileItem MidiFIO::read() {
        MidiFileItem returnitem;
        return returnitem;
    }

    bool MidiFIO::open(std::string file, std::string mode) {
        if (river.is_open())
            return true;
        WarningPopup* awarning;
        if (file.substr(file.size()-5,4) != ".mid") {
            awarning = new WarningPopup;
            awarning->setWarningText("Wrong File Extension");
            awarning->setInfoText(std::string(file) + " might not be a MIDI file due to its extension (which is not .mid).\n\nBy ignoring this warning, the file will be read anyway.\n\nOtherwise, the import will be canceled.\n");
            awarning->exec();
            bool goon = awarning->returnContinue();
            delete awarning;
            if (!goon)
                return false;
        }
        if (mode == "r") {
            bool ignoreInsane = false;

            auto fileIsInsane = [&ignoreInsane, &awarning, file]()->bool {
                if (ignoreInsane)
                    return false;
                awarning = new WarningPopup;
                awarning->setWarningText("Insane MIDI File");
                awarning->setInfoText(std::string(file) + " appears to be either corrupt or non-conformant to the MIDI standard.\n\nBy ignoring this warning, Soundbench will attempt to continue parsing the file.\n\nOtherwise, the import will be canceled.\n");
                awarning->exec();
                bool goon = awarning->returnContinue();
                delete awarning;
                if (goon)
                    ignoreInsane = true;
                return !goon;
            };

            river.open(file,std::ios_base::in|std::ios_base::binary);

            //Sanity check.
            for (unsigned char i = 0; i < 9; ++i) {
                static const char* chr = "Mtrk\0\0\0\6\0";
                if(river.get() != chr[i]) {
                    if (fileIsInsane())
                        return false;
                }
            }

            filetype = river.get();
            if(filetype == 0) {
                river.ignore(2); //The next unsigned 16-bit integer will always be 1 in this case.
                tracks.push_back(14); //TODO: Replace this with a sanity check.
            }

            else {
                int trks = river.get();
                trks <<= 4;
                trks += river.get();
                tracks.reserve(trks);
                river.seekg(14);

                //Index the offsets of all the files.
                while (river.good()) {
                    //TODO: Write this.
                }

                if(river.eof())
                    river.clear();
                else
                    return false;

                //Sanity checks.
                bool sanefile = true;
                if((tracks[0] != 15) && !ignoreInsane)
                    sanefile = !fileIsInsane();
                if (!sanefile)
                    return false;
            }
            writing = false;
            res = river.get();
            res <<= 4;
            res += river.get();
            res_is_fps = res & 128; //Get the first bit.
            res &= 127; //Set the first bit to 0.
        }
        else if (mode == "w") {
            river.open(file,std::ios_base::out|std::ios_base::trunc|std::ios_base::binary);
            writing = true;
            if(river.is_open()) {
                river.write("MThd\0\0\0\6\0\0\0\1\3\192",14); //Write the part of the header that will always be the same.
                //The header (fancily formatted) is:  MThD 6 0 1 960
                river.write("MTrk\0\0\0\0",8); //The first part of the header. For now, the size of 0 is a placeholder value.
                //Change the four bytes at the offsets of 18-22 to the size when all is said and done.
            }
            tracklen = 0;
        }
        if(!river.is_open()) {
        }
        return true;
    }

    bool MidiFIO::close() {
        if (river.is_open()) {
            if (writing) {
                //TODO: Fill bytes 18-22 with tracklen.
            }
            river.close();
        }
        return true;
    }
}
