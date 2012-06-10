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

#ifndef BACKEND_H
#define BACKEND_H

#include "sbutilities.h"
#include "synth.h"

//I'd have liked to keep the Synth and backend class of objects separate, but it ends up being a lot easier to maintain the required performance this way.

namespace sb {

    class EmitterBackend {
    public:
        EmitterBackend() {
            running = false;
        }
        virtual ~EmitterBackend() {}
        virtual void stop() = 0;
        virtual void start() = 0;
        virtual void setSamplingRate(size_t) = 0;
        virtual bool isRunning() {
            return running;
        }

    protected:
        size_t sampling_rate;
        sb::Synth* syn;
        bool running;
    };

}

#endif // BACKEND_H
