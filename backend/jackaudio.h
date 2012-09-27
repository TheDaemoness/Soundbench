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

#include "backend/base.h"

#ifndef JACKAUDIO_H
#define JACKAUDIO_H

#ifndef NO_JACK
namespace sb {
    class JackAudioBackend : public EmitterBackend {
    public:
        static bool instantiable();
        explicit JackAudioBackend(sb::Synth*, size_t&, std::map<size_t,bool>&, size_t);
        size_t returnSuggestedBufferSize();
        ~JackAudioBackend();

        void start();
        void stop();
        size_t getDefaultDevice();
        size_t getCurrentDevice();

        void setSamplingRate(size_t);
        void setDevice(uint32_t devi);
        std::vector<std::string> getDevices();
    };
}
#else
namespace sb {
    class JackAudioBackend : public EmitterBackend {
    public:
        static bool instantiable();
        explicit JackAudioBackend(sb::Synth*, size_t&,std::map<size_t,bool>&,size_t) {}
        size_t returnSuggestedBufferSize() {return 0;}
        ~JackAudioBackend() {}
        void start() {}
        void stop() {}
        size_t getDefaultDevice() {return 0;}
        size_t getCurrentDevice() {return 0;}
        void setSamplingRate(size_t) {}
        void setDevice(uint32_t) {}
        std::vector<std::string> getDevices() {return std::vector<std::string>();}
    };
}
#endif

#endif // JACKAUDIO_H
