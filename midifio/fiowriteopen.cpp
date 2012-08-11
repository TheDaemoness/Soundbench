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
        bool MidiFIO::writerOpen(std::string file)
#else
        bool MidiFIO::writerOpen(std::string file, std::ostream& error_stream)
#endif
        {
            if (river.is_open())
                return false;
            river.open(file,std::ios_base::out|std::ios_base::trunc|std::ios_base::binary);
            if (!river.is_open()) {
#ifdef IS_SOUNDBENCH
                ErrorPopup* anerror;
                anerror = new ErrorPopup;
                anerror->setErrorText("Unable to Open File");
                anerror->setInfoText(std::string("Soundbench could not open ") + file + ". This may be because the directory it's to be written to doesn't exist or because you do not have permissions to open it for writing.\n");
                anerror->exec();
                delete anerror;
#else
                error_stream << "Could not open the file for writing.\n";
#endif
                return false;
            }

            river.write("MThd\0\0\0\6\0\0\0\1",12); //Write the header for a single-track MIDI file.
            river.write("MTrk\0\0\0\0",8); //Write an empty track header.

            writing = true;
            tracklen = 0;

            return true;
        }
    }
}
