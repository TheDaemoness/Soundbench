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
    bool Emitter::initSomeBackend(EmitterType notthisone) {
        for (auto pair : supported_apis) {
            if (pair.first == PortAudio_O && notthisone != PortAudio_O) {
                if (initPortAudio())
                    return true;
            }
            else if (pair.first == RtAudio_O && notthisone != RtAudio_O) {
                if (initRtAudio())
                    return true;
            }
        }
        backend = nullptr;
        return false;
    }
}
