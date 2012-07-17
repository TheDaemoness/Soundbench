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

#ifndef BACKEND_PORTAUDIO_H
#define BACKEND_PORTAUDIO_H

#include "backend/base.h"

//NOTICE: Do not reimplement this class to use Pa_WriteStream. It uses the callback to take advantage of the high-priority thread it runs in.


#ifndef NO_PORTAUDIO
#include <portaudio.h>

namespace sb {
    class PortaudioBackend : public EmitterBackend {
    public:
        static bool instantiable();
        static bool pa_inited;
        explicit PortaudioBackend(sb::Synth*, size_t&, std::map<size_t,bool>&, size_t);
        size_t returnSuggestedBufferSize();
        ~PortaudioBackend();

        void start();
        void stop();
        size_t getDefaultDevice(){
            return Pa_GetDefaultOutputDevice();
        }
        size_t getCurrentDevice() {
            return dev.device;
        }

        void setSamplingRate(size_t);
        void setDevice(uint32_t devi) {
            dev.device = devi;
        }

        std::vector<std::string> getDevices();

        static int callback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);

    private:
        bool dead;
        PaStream* river;
        PaStreamParameters dev;

    };
}
#else
namespace sb {
    class PortaudioBackend : public EmitterBackend {
    public:
        static bool instantiable();
        explicit PortaudioBackend(sb::Synth*, size_t&,std::map<size_t,bool>&,size_t) {};
        size_t returnSuggestedBufferSize() {return 0;}
        ~PortaudioBackend() {}
        void start() {}
        void stop() {}
        size_t getDefaultDevice() {return 0;}
        size_t getCurrentDevice() {return 0;}
        void setSamplingRate(size_t) {}
        void setDevice(uint32_t) {};
        std::vector<std::string> getDevices() {return std::vector<std::string>();}
    };
}
#endif

#endif // BACKEND_PORTAUDIO_H
