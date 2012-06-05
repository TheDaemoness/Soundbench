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

#include "midirtio.h"

namespace sb {

    MidiRtIO::MidiRtIO(Synth* s) {
        syn = s;
        failure = false;

        supported_apis[PortMIDI] = PortmidiFrontend::instantiable();

        if(supported_apis[PortMIDI])
            type = PortMIDI;
        else {
            std::cerr << "No real time MIDI frontends can be initialized on this computer.\n";
            type = NoMIDI;
        }

        frnt = nullptr;
        setFrontendType(type);
    }

    void MidiRtIO::setFrontendType(FrontendType type) {
        if (type == PortMIDI) {
            std::cerr << "Initializing a PortMIDI midi frontend...\n";
            frnt = new PortmidiFrontend(syn);
            std::cerr << "PortMIDI frontend initialized.\n";
        }
    }

    MidiRtIO::~MidiRtIO() {
        if (frnt != nullptr)
            delete frnt;
    }

}
