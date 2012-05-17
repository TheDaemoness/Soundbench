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
    bool Player::writeFile(std::string fname) {
        reed->close();
        std::string ext = fname.substr(fname.find_last_of("."));
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
                return false;
            else
                form = FileWAV;
        }
        if(!wri->open(fname,form))
            return false;

        midi::MIDIEventNode* nody = first;
        uint32_t evcount = 0;
        while (true) {
            if (nody == nullptr)
                break;
            else
                nody = nody->returnNext();
            ++evcount;
        }

        affectedmet->setProgress(0);

        //Microseconds per sample: (microseconds / second) / (samples / second)
        uint32_t factor = 100000 / curr_srate;
        bool cont = true;
        nody = first;
        float evnum = 0;

        for(uint64_t microsecs = 0; cont; microsecs += factor) {
            while (microsecs >= nody->getDelay()) {
                nody->doEvent();
                microsecs -= nody->getDelay();
                nody = nody->returnNext();
                if (nody == nullptr)
                    cont = false;
            }
            ++evnum;
            affectedmet->setProgress(evnum/evcount*1000);
            wri->tick();
        }

        affectedmet->startMeter();
        return true;
    }
}
