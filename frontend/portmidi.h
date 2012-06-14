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

#ifndef FRONTEND_PORTMIDI_H
#define FRONTEND_PORTMIDI_H

#include "frontend/base.h"

namespace sb {

#ifndef NO_PORTMIDI
#include <portmidi.h>

    class PortmidiFrontend : public MidiFrontend {
    public:
        static bool instantiable();
        PortmidiFrontend(Synth* s);
        ~PortmidiFrontend();
        void start();
        void stop();
        static PmTimestamp callback(void* data);
    private:
        PortMidiStream* river;
        std::chrono::time_point<std::chrono::high_resolution_clock> starttime;
    };

#else
    class PortmidiFrontend : public MidiFrontend {
    public:
        static bool instantiable();
        PortmidiFrontend(Synth* s) : MidiFrontend(s) {};
        void start() {};
        void stop() {};
    };
#endif

}

#endif // FRONTEND_PORTMIDI_H
