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
        bool MidiFIO::open(std::string file, std::string mode) {
            if (river.is_open())
                return true;
            WarningPopup* awarning;
            if (file.substr(file.size()-4,4) != ".mid" && file.substr(file.size()-4,4) != ".smf" && mode == "r") {
                awarning = new WarningPopup;
                awarning->setWarningText("Wrong File Extension");
                awarning->setInfoText(std::string(file) + " might not be a MIDI file due to its extension (which is not .mid or .smf).\n\nBy ignoring this warning, the file will be read anyway.\n\nOtherwise, the import will be canceled.\n");
                awarning->exec();
                bool goon = awarning->returnContinue();
                delete awarning;
                if (!goon)
                    return false;
            }
            if (mode == "r") {
                writing = false;
                std::cerr << "Opening MIDI file " << file << " for reading...\n";
                bool ignoreInsane = false;

                auto fileIsInsane = [&ignoreInsane, &awarning, file, this](std::string reason)->bool {
                    if (ignoreInsane)
                        return false;
                    awarning = new WarningPopup;
                    awarning->setWarningText("Invalid MIDI File");
                    if (reason.size() == 0)
                        awarning->setInfoText(file + " appears to be either corrupt or non-conformant to the MIDI standard.\n\nBy ignoring this warning, Soundbench will attempt to continue parsing the file.\n\nOtherwise, the import will be canceled.\n");
                    else
                        awarning->setInfoText(file + " appears to be either corrupt or non-conformant to the MIDI standard because " + reason + ".\n\nBy ignoring this warning, Soundbench will attempt to continue parsing the file.\n\nOtherwise, the import will be canceled.\n");
                    awarning->exec();
                    bool goon = awarning->returnContinue();
                    delete awarning;
                    if (goon)
                        ignoreInsane = true;
                    else
                        river.close();
                    return !goon;
                };

                ErrorPopup* anerror;
                river.open(file,std::ios_base::in|std::ios_base::binary);
                if (!river.is_open()) {
                    anerror = new ErrorPopup;
                    anerror->setErrorText("Unable to Open File");
                    anerror->setInfoText(std::string("Soundbench could not open '") + file + "'. This may be because the file does not exist or because you do not have permissions to open it.\n\nAt the moment, Soundbench has no way to change the permissions of files you do not have the rights to access.");
                    anerror->exec();
                    delete anerror;
                    return false;
                }
                std::cerr << "Opened file successfully. Indexing and checking sanity...\n";

                //Sanity check: is the main header well-formed?
                for (unsigned char i = 0; i < 9; ++i) {
                    static const char* chr = "MThd\0\0\0\6\0";
                    if(river.get() != chr[i]) {
                        if (fileIsInsane("the file has a mal-formed header"))
                            return false;
                    }
                }

                //Get the type of the file.
                filetype = river.get();
                std::cerr << "File is of type " << static_cast<char>(filetype + '0') << ".\n";
                if(filetype == 0) {
                    river.ignore(2); //The next unsigned 16-bit integer will always be 1 in this case.
                }
                else {
                    if (filetype != 1 && filetype != 2) {
                        if (fileIsInsane("the file is not a type 0, 1, or 2 MIDI file"))
                            return false;
                    }
                    int trks = river.get();
                    trks <<= 8;
                    trks += river.get();
                    tracks.reserve(trks);
                    tracklen = trks;
                }

                //Get the time data.
                writing = false;
                res = river.get();
                res <<= 8;
                res += river.get();
                res_is_fps = res & 128; //Get the first bit.
                res &= 127; //Set the first bit to 0.

                //Sanity check: Is the first track's header sane?
                for (unsigned char i = 0; i < 4; ++i) {
                    static const char* chrs = "MTrk";
                    if(river.get() != chrs[i] || river.eof()) {
                        if (fileIsInsane("the file does not have a valid track just after the header"))
                            return false;
                    }
                }
                tracks.push_back(river.tellg()-4L);

                //Index all the tracks.
                if (filetype != 0) {
                    while (river.good() && tracks.size() < tracklen) {
                        uint32_t bites = 0; //Har har.
                        for (unsigned char i = 0; i < 3; ++i) {
                            bites += river.get();
                            bites <<= 8;
                        }
                        bites += river.get();
                        river.seekg(bites,std::ios_base::cur);

                        //Sanity check: After the number of given bytes, does an MTrk or EOF exist?
                        unsigned char i = 0;
                        for (; i < 4 && river.good(); ++i) {
                            static const char* chrs = "MTrk";
                            if(river.get() != chrs[i] && !river.eof()) {
                                if (fileIsInsane("one of the tracks has a malformed or incorrect header"))
                                    return false;
                            }
                            else if (river.eof() && i > 0) {
                                if (fileIsInsane("the last track's header is incomplete"))
                                    return false;
                            }
                            else if (river.eof() && i == 0)
                                break;
                        }
                        //TODO: For future versions, have a way of checking for an end-of-track event.
                        if(!river.eof())
                            tracks.push_back(river.tellg()-4L);

                    }
                }
                if(river.eof()) {
                    river.seekg(14);
                    river.clear();
                }

                if (tracks.size() < tracklen) {
                    if (fileIsInsane("fewer tracks are present in the MIDI file than expected"))
                        return false;
                }
                std::cerr << "File indexed" << (ignoreInsane?".\n":" and (probably) sane.\n");
            }
            else if (mode == "w") {
                std::cerr << "Opening MIDI file " << file << " for writing...\n";
                river.open(file,std::ios_base::out|std::ios_base::trunc|std::ios_base::binary);
                if (!river.is_open()) {
                    //TODO: Complain, and have a retry loop.
                }
                std::cerr << "File opened successfully.\n";

                writing = true;
                river.write("MThd\0\0\0\6\0\0\0\1\3\192",14); //Write the part of the header that will always be the same.
                //The header (fancily formatted) is:  MThD 6 0 1 960
                river.write("MTrk\0\0\0\0",8); //The first part of the header. For now, the size of 0 is a placeholder value.
                //Change the four bytes at the offsets of 18-22 to the size when all is said and done.
                tracklen = 0;
            }
            if(!river.is_open()) {
            }
            return true;
        }
    }
}
