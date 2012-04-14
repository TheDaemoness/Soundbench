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
    namespace midi {

        void MidiFIO::write(MidiFileItem writeitem) {
            if (!writing || !river.is_open())
                return;
            //TODO: The following two lines are a hack. In a future version, Soundbench will correctly store the full range of values of a uint32_t.
            if (writeitem.delay & 4026531840) { // The most significant 4 bits of a uint32_t.
                std::cerr << "Internal warning: one or more of the last significant bits of a MidiFileItem::delay were set.\n";
                river.write("\255\255\255\127",4);
            }
            else {
                uint8_t len = 0;
                //TODO: Messy code. Clean this up.
                if (writeitem.delay < 256)
                    len = 1;
                else if (writeitem.delay < 16384)
                    len = 2;
                else if (writeitem.delay < 1048576)
                    len = 3;
                else
                    len = 4;
                tracklen += len;
                writeitem.delay <<= len + (4-len)*8;
                for(uint8_t i = 0; i < len; ++i) {
                    writeitem.delay >>= 1;
                    uint8_t val = *(reinterpret_cast<uint8_t*>(&writeitem.delay)+i);
                    if (i == len-1)
                        val &= 127; //Set the next-byte-exists bit to 0.
                    else
                        val |= 128; //Set the next-byte-exists bit to 1.
                    river.put(val);
                }
            }
            uint8_t dat = writeitem.evtype;
            dat <<= 4;
            dat += writeitem.chan;
            river.put(dat);
            if(writeitem.evtype != midi::Meta) {
                river.put(writeitem.params.first);
                river.put(writeitem.params.second);
            }
            else {
                river.put(writeitem.meta);
                if (writeitem.meta == midi::EndOfTrack) {
                    river.put(0);
                }
            }
        }

        void MidiFIO::readfrom(uint16_t traque) {
            if(river.is_open() && !writing && traque < tracks.size())
                river.seekg(tracks[traque]+4); //Soundbench can ignore the 4 bytes with the Mtrk.
            tracklen = (trackpos = 0);
            for (unsigned char i = 0; i < 4; ++i) {
                tracklen += river.get();
                if (i < 3)
                    tracklen <<= 8;
            }
        }

        MidiFileItem MidiFIO::read() {
            MidiFileItem returnitem;
            returnitem.evtype = midi::Failed;
            if (writing || !river.is_open())
                return returnitem;
            if (trackpos >= tracklen) {
                returnitem.evtype = midi::EndOfTrack;
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
            returnitem.chan = byte & 31; //Mask out the event type bits.
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

        bool MidiFIO::open(std::string file, std::string mode) {
            if (river.is_open())
                return true;
            WarningPopup* awarning;
            if (file.substr(file.size()-5,4) != ".mid" && file.substr(file.size()-5,4) != ".smf" && mode == "r") {
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
                std::cerr << "Opening MIDI file " << file << " for reading...\n";
                bool ignoreInsane = false;

                auto fileIsInsane = [&ignoreInsane, &awarning, file](std::string reason)->bool {
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
                std::cerr << "Opened file successfully. Parsing and checking sanity...";

                //Sanity check: is the main header well-formed?
                for (unsigned char i = 0; i < 9; ++i) {
                    static const char* chr = "Mthd\0\0\0\6\0";
                    if(river.get() != chr[i]) {
                        if (fileIsInsane("the file has a mal-formed header"))
                            return false;
                    }
                }

                //Get the type of the file.
                filetype = river.get();
                if(filetype == 0) {
                    river.ignore(2); //The next unsigned 16-bit integer will always be 1 in this case.
                }
                else {
                    if (filetype > 0) {
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
                    static const char* chrs = "Mtrk";
                    if(river.get() != chrs[i] || river.eof()) {
                        if (fileIsInsane("the file does not have a valid track just after the header"))
                            return false;
                    }
                }

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

                        //Sanity check: After the number of given bytes, does an Mtrk or EOF exist?
                        bool lastheaderfine = true;
                        unsigned char i = 0;
                        for (; i < 4 && river.good(); ++i) {
                            static const char* chrs = "Mtrk";
                            if(river.get() != chrs[i] && !river.eof()) {
                                lastheaderfine = false;
                                if (fileIsInsane("one of the tracks has a malformed or incorrect header"))
                                    return false;
                            }
                            else if (river.eof() && i > 0) {
                                lastheaderfine = false;
                                if (fileIsInsane("the last track's header is incomplete"))
                                    return false;
                            }
                            else if (river.eof() && i == 0)
                                break;
                        }
                        if (lastheaderfine)
                            tracks.push_back(river.tellg()-(4L-i-bites));
                        //TODO: If this part of the file was insane, have a way to check for an end-of-track event to get the real size.
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
                std::cerr << "File parsed" << (ignoreInsane?".\n":" and sane.\n");
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

        bool MidiFIO::close() {
            if (river.is_open()) {
                if (writing) {
                    river.seekp(18);
                    river.write(reinterpret_cast<char*>(&tracklen),4);
                }
                river.close();
            }
            return true;
        }
    }
}
