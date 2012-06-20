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

#include "emitter.h"

namespace sb {
    bool Emitter::initRtAudio() {
        used_backend = RtAudio_O;
        if (supported_apis[sb::RtAudio_O]) {
            std::cerr << "Initializing an RtAudio audio backend...\n";
            backend = new RtAudioBackend(syn,sample_rate,supported_rates,OutChannels);
            if (backend->isReady()) {
                std::cerr << "RtAudio backend initialized.\n";
                return true;
            }
            else {
                delete backend;
                std::cerr << "Cannot initialize the RtAudio audio backend.\n";
                return false;
            }
        }
        else {
            std::cerr << "Cannot initialize the RtAudio audio backend.\n";
            return false;
        }
    }
}
