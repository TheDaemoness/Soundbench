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

#ifdef IS_SOUNDBENCH
        bool MidiFIO::readerOpen(std::string file)
#else
        bool MidiFIO::readerOpen(std::string file, std::ostream& error_stream)
#endif
        {
            if (river.is_open()) {
                return false;
            }
            eot_reached = false;
            factor = 0;

            WarningPopup* awarning;
            if (file.substr(file.size()-4,4) != ".mid" && file.substr(file.size()-4,4) != ".smf") {
#ifdef IS_SOUNDBENCH
                awarning = new WarningPopup;
                awarning->setWarningText("Wrong File Extension");
                awarning->setInfoText(std::string(file) + " might not be a MIDI file due to its extension (which is not .mid or .smf).\n\nBy ignoring this warning, the file will be read anyway.\n\nOtherwise, the import will be canceled.\n");
                awarning->exec();
                bool goon = awarning->fixed();
                delete awarning;
                if (!goon)
                    return false;
#endif
            }
            writing = false;
            bool ignoreInsane = false;

            auto fileIsInsane = [&ignoreInsane, &awarning, file, this](std::string reason)->bool {
#ifdef IS_SOUNDBENCH
                if (ignoreInsane)
                    return false;
                awarning = new WarningPopup;
                awarning->setWarningText("Invalid MIDI File");
                if (reason.size() == 0)
                    awarning->setInfoText(file + " appears to be either corrupt or non-conformant to the MIDI standard.\n\nBy ignoring this warning, Soundbench will attempt to continue parsing the file.\n\nOtherwise, the import will be canceled.\n");
                else
                    awarning->setInfoText(file + " appears to be either corrupt or non-conformant to the MIDI standard because " + reason + ".\n\nBy ignoring this warning, Soundbench will attempt to continue parsing the file.\n\nOtherwise, the import will be canceled.\n");
                awarning->exec();
                bool goon = awarning->fixed();
                delete awarning;
                if (goon)
                    ignoreInsane = true;
                else
                    river.close();
                return !goon;
#else
                if (reason.size() == 0)
                    error_stream << "Unknown error.\n";
                else {
                    reason[0] = toupper(reason[0]);
                    error_stream << reason << '.';
                }
                river.close();
                return false;
#endif
            };

            ErrorPopup* anerror;
            river.open(file,std::ios_base::in|std::ios_base::binary);
            if (!river.is_open()) {
#ifdef IS_SOUNDBENCH
                anerror = new ErrorPopup;
                anerror->setErrorText("Unable to Open File");
                anerror->setInfoText(std::string("Soundbench could not open ") + file + ". This may be because the file does not exist or because you do not have permissions to open it.\n\nAt the moment, Soundbench has no way to change the permissions of files you do not have the rights to access.");
                anerror->exec();
                delete anerror;
                return false;
#else
                error_stream << '\'' << file << "' cannot be opened for reading.";
#endif
            }

#ifdef IS_SOUNDBENCH
            std::cerr << "Indexing the file...\n";
#endif

            //Sanity check: is the main header well-formed?
            for (unsigned char i = 0; i < 9; ++i) {
                static const char* chr = "MThd\0\0\0\6\0";
                if(river.get() != chr[i]) {
                    if (fileIsInsane("the file has a header that is not MIDI-conformant"))
                        return false;
                }
            }

            //Get the type of the file.
            filetype = river.get();
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
            uint16_t raw_res;
            raw_res = river.get();
            raw_res <<= 8;
            raw_res += river.get();

            //Parse the time data
            bool res_is_fps = (raw_res >> 8) & Bit1;
            raw_res = (raw_res << 1) >> 1;
            if (res_is_fps) {
                factor = 1000000/(raw_res & 65280)/(raw_res & 255); //microseconds/second * seconds/frame * frames/tick
                ticks_per_beat = 0;
            }
            else {
                factor = 500000/raw_res; //microseconds/beat * beats/tick
                ticks_per_beat = raw_res;
            }

            //Sanity check: Is the first track's header sane?
            const char* chrs = "MTrk";
            for (unsigned char i = 0; i < 4; ++i) {
                if(river.get() != chrs[i] || river.eof()) {
                    if (fileIsInsane("the file does not have a valid track just after the header"))
                        return false;
                }
            }
            tracks.push_back(static_cast<size_t>(river.tellg())-4LL);

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
                        tracks.push_back(static_cast<size_t>(river.tellg())-4LL);
                }
            }

            if (tracks.size() < tracklen) {
                if (fileIsInsane("fewer tracks are present in the MIDI file than expected"))
                    return false;
            }
            readFrom(0);

#ifdef IS_SOUNDBENCH
            std::cerr << "File opened and indexed.\n";
#endif

            if(filetype == MultiTrack) { //Handle the meta events written in the first track.
                std::cerr << "Parsing the metadata track.\n";
                MidiFileItem ret;
                do {
                    ret = read();
                } while (ret.evtype != EndOfTrack);
                readFrom(1);
                std::cerr << "Done parsing the metadata track.\n";
            }

            return true;
        }
    }
}

