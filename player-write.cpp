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


#include "player.h"

namespace sb {
    void Player::writeFile() {
        std::string ext = fi.substr(fi.find_last_of("."));
        ExportableFiles form;
        if(ext == ".wav")
            form = FileWAV;
        else if(ext == ".aiff")
            form = FileAIFF;
        else if(ext == ".flac")
            form = FileFLAC;
        else if(ext == ".raw")
            form = FileRAW;
        else if(ext == ".mat")
            form = FileMAT;
        else {
            WarningPopup* w = new WarningPopup;
            w->setWarningText("Unknown File Format");
            w->setInfoText("Soundbench was asked to export a file in a format that it isn't familiar with.\n\nIf you choose to ignore this warning, Soundbench will default to exporting a .wav file using the given extension.");
            w->exec();
            bool goon = w->fixed();
            delete w;
            if (!goon)
                return;
            else
                form = FileWAV;
        }
        if(!wri->open(fi,form))
            return;

        midi::MIDIEventNode* nody = first;
        if (nody == nullptr)
            return;

        uint32_t evcount = 0;
        while (nody->returnNext() != nullptr) {
            nody = nody->returnNext();
            ++evcount;
        }

        if (evcount == 1)
            std::cerr << "Have 1 event to process.\n";
        else
            std::cerr << "Have " << evcount << " events to process.\n";
        emit progressed(0);

        //Microseconds per sample: (microseconds / second) / (samples / second)
        uint16_t factor = 1000000 / curr_srate;
        nody = first;
        bool cont = true;
        uint32_t evnum = 0, prevnum = 0;
        uint64_t sampcount = 0;

        for(uint64_t microsecs = 0; cont; microsecs += factor) {
            if(!wri->empty() && microsecs >= nody->getDelay())
                wri->flush();
            while (microsecs >= nody->getDelay()) {
                nody->doEvent();
                microsecs -= nody->getDelay();
                nody = nody->returnNext();
                if (nody == nullptr) {
                    cont = false;
                    break;
                }
                ++evnum;
            }
            ++sampcount;
            if (evnum != prevnum) {
                emit progressed(1000*evnum/evcount);
                prevnum = evnum;
            }
            wri->tick();
        }

        std::cerr << "Wrote " << sampcount << " samples to the file (appx. " << static_cast<float>(sampcount)*2 / curr_srate << " seconds).\n";
        wri->close();
        return;
    }
}
