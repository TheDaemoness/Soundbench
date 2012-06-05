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

#ifndef BACKEND_JACK_H
#define BACKEND_JACK_H

#include "backend.h"

#ifndef NO_JACK
namespace sb {
    class JACKBackend {
    public:
        static bool instantiable();
        explicit JACKBackend(sb::Synth*, size_t&,std::map<size_t,bool>&,size_t);
        void start();
        void stop();
        void setSamplingRate(size_t);
        size_t returnSuggestedBufferSize() {return 0;}
    };
}
#else
namespace sb {
    class JACKBackend {
    public:
        static bool instantiable();
        explicit JACKBackend(sb::Synth*, size_t&,std::map<size_t,bool>&,size_t) {};
        void start() {};
        void stop() {};
        void setSamplingRate(size_t) {};
        size_t returnSuggestedBufferSize() {return 0;}
    };
}
#endif

#endif // BACKEND_JACK_H
