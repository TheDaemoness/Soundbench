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

    void Emitter::setEmitterType(emitter_type emt) {
        if (backend != nullptr) {
            delete backend;
        }
        if (emt == sb::PortAudio) {
            std::clog << "Initializing PortAudio as the audio backend...\n";
            backend = new portaudio_backend(syn, sample_rate,supported_rates,2);
            std::clog << "PortAudio backend initialized.\n";
        }
        else
            std::__throw_logic_error("emitter - Unimplemented backend.");
    }

    Emitter::Emitter(Synth* s) {
        this->supported_apis[sb::PortAudio] = portaudio_backend::instantiable();
        syn = s;
        em_type = sb::PortAudio;
        sample_rate = sb::curr_srate;
        backend = nullptr;
        setEmitterType(em_type);
    }

    void Emitter::setSamplingRate(size_t s_rate) {
        curr_srate = s_rate;
        if (backend != nullptr)
            backend->setSamplingRate(s_rate);
        std::clog << "Set sampling rate to " << curr_srate << " samples per second.\n";
    }

    Emitter::~Emitter() {
        if (backend != nullptr)
            delete backend;
    }
}
