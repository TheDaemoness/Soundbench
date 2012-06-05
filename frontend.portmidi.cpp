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

#include "frontend.portmidi.h"
#include <chrono>

namespace sb {

#ifndef NO_PORTMIDI
    bool PortmidiFrontend::instantiable() {
        PmError e = Pm_Initialize();
        if(e != pmNoError)
            return false;
        if(Pm_CountDevices() == 0) {
            Pm_Terminate();
            return false;
        }
        Pm_Terminate();
        return true;
    }

    PmTimestamp PortmidiFrontend::callback(void* data) {
        std::chrono::time_point<std::chrono::high_resolution_clock> t = *reinterpret_cast<decltype(t)*>(data);
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-t).count();
    }

    PortmidiFrontend::PortmidiFrontend(Synth *s) : MidiFrontend(s) {
        Pm_Initialize();
        river = nullptr;
    }
    PortmidiFrontend::~PortmidiFrontend() {
        stop();
        Pm_Terminate();
    }

    void PortmidiFrontend::start() {
        if (running)
            return;
        starttime = std::chrono::high_resolution_clock::now();
        PmError e = Pm_OpenInput(&river,Pm_GetDefaultInputDeviceID(),NULL,10,callback,&starttime);
        if (e != pmNoError) {
            river = nullptr;
            return;
        }
        running = false; //TODO: Remove this when it actually runs.
    }
    void PortmidiFrontend::stop() {
        if (!running)
            return;
        Pm_Close(river);
        river = nullptr;
        running = false;
    }

#else
    bool PortmidiFrontend::instantiable() {
        return false;
    }
#endif

}
