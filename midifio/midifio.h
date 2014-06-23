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

#ifdef IS_SOUNDBENCH
#include "err/errorpopup.h"
#include "err/warningpopup.h"
#endif

#include "common.h"
#include "midievents.h"

namespace sb {
	namespace midi {

		class MidiFIO
		{
		public:
			MidiFIO() {
				returnitem.evtype = midi::NoData;
				writing = false;
				tracklen = 0;
				eot_reached = false;
			}
			~MidiFIO() {
				river.close();
			}

#ifndef IS_SOUNDBENCH
			bool readerOpen(std::string, std::ostream& error_stream = std::cerr);
#else
			bool readerOpen(std::string);
#endif
			bool readFrom(uint16_t); //Changes from which track read() reads.
			std::string getTrackName(uint16_t); //Checks the specified track for a meta-event immediately after the header giving the track's name.
			MidiEvent read();
			bool close();

			uint16_t getTempo();
			bool setTempo(uint16_t);

			inline uint16_t getTrackCount() {
				return (filetype?tracks.size():1);
			}
			inline MidiFileTypes getFileType() {
				return static_cast<MidiFileTypes>(filetype);
			}
			inline bool isOpen() {
				return river.is_open();
			}

		private:
			char filetype;
			std::vector<size_t> tracks; //Stores the read pointer offset to read from each track, starting with the Mtrk.
			std::fstream river;
			bool writing, eot_reached;
			uint32_t factor; //microseconds/tick
			uint16_t ticks_per_beat;
			size_t tracklen;
			MidiEvent returnitem;
		};
	}
}

#endif // MIDIREADER_H
