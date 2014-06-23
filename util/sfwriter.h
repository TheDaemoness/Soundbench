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

#ifndef SFRENDERER_H
#define SFRENDERER_H

#include <sndfile.h>

#include <deque>

#include "emitter/emitter.h"
#include "synth/synth.h"

namespace sb {

	enum ExportableFiles {
		FileWAV,
		FileAIFF,
		FileFLAC,
		FileRAW,
		FileMAT
	};

	/*
	WARNING: This is a class that will react poorly to a change of the type of sbSample.
	For reasons why, see the put and tick functions of this class.
*/
	class SoundFileWriter {
	public:
		explicit SoundFileWriter(Synth* seen) {
			filehandel = nullptr;
			syn = seen;
			needsflush = false;
		}

		bool open(std::string name, ExportableFiles ex);
		void put(SbSample* samp, size_t len = 1);
		void tick();
		bool flush();
		inline bool is_open() {
			return filehandel != nullptr;
		}

		bool empty() {
			return !needsflush;
		}

		void close();

		~SoundFileWriter() {
			close();
		}
	private:
		std::deque<SbSample> samples;
		bool emstate, needsflush;
		SNDFILE* filehandel;
		Synth* syn;
	};

}

#endif // SFRENDERER_H
