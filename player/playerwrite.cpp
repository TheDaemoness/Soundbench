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
		std::string ext;
		if (fi.find_last_of(".") == std::string::npos) {
			ext = ".wav";
			std::cerr << "Warning: No extension given. Defaulting to '" << ext << "'...\n";
			fi += ext;
		}
		else
			ext = fi.substr(fi.find_last_of("."));

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

		midi::nodes::MIDIEventNode* nody = first;
		if (nody == nullptr)
			return;

		uint32_t evcount = 0;
		while (nody->returnNext() != nullptr) {
			nody = nody->returnNext();
			++evcount;
		}

		--evcount;
		if (evcount == 1)
			std::cerr << "Have 1 event to process (excluding the end-of-track event).\n";
		else
			std::cerr << "Have " << evcount << " events to process (excluding the end-of-track event).\n";
		emit progressed(0);

		//Microseconds per sample: (microseconds / second) / (samples / second)
		uint16_t factor = 1000000 / global_srate;
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

		uint64_t playtime = static_cast<float>(sampcount) / global_srate;
		if (playtime == 1)
			std::cerr << "Wrote " << sampcount << " samples to the file (appx. 1 second).\n";
		else if (playtime < 60)
			std::cerr << "Wrote " << sampcount << " samples to the file (appx. " << playtime << " seconds).\n";
		else if (playtime == 61)
			std::cerr << "Wrote " << sampcount << " samples to the file (appx. 1 minute and 1 second).\n";
		else if (playtime >= 60 && playtime < 120)
			std::cerr << "Wrote " << sampcount << " samples to the file (appx. 1 minute and " << playtime%60 << " seconds).\n";
		else if (playtime % 60 == 1)
			std::cerr << "Wrote " << sampcount << " samples to the file (appx. " << playtime/60 << " minutes and 1 second).\n";
		else
			std::cerr << "Wrote " << sampcount << " samples to the file (appx. " << playtime/60 << " minutes and " << playtime%60 << " seconds).\n";
		wri->close();
		return;
	}
}
