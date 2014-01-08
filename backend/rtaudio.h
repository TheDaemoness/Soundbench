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

#ifndef BACKEND_RTAUDIO_H
#define BACKEND_RTAUDIO_H

#include "backend/base.h"

#ifndef NO_RTAUDIO
#include <RtAudio.h>

namespace sb {
    class RtAudioBackend : public EmitterBackend {
    public:
        static bool instantiable();
        explicit RtAudioBackend(sb::Synth*, size_t&, std::map<size_t,bool>&, size_t);
        ~RtAudioBackend() {}

        void start();
        void stop();

        bool isRunning() {
            return rta.isStreamRunning();
        }

        size_t getDefaultDevice(){
            return rta.getDefaultOutputDevice();
        }
        size_t getCurrentDevice() {
            return params.deviceId;
        }

        void setSamplingRate(size_t neorate) {
            stop();
            if(rta.isStreamOpen())
                rta.closeStream();
            sampling_rate = neorate;
            bufflen = sampling_rate/100;
            rta.openStream(&params,NULL,RTAUDIO_FLOAT32,sampling_rate,&bufflen,callback,reinterpret_cast<void*>(syn));
            ready = rta.isStreamOpen();
        }
        void setDevice(uint32_t dev) {
            stop();
            if (!dev)
                params.deviceId = rta.getDefaultOutputDevice();
            else
                params.deviceId = dev;
            if(rta.isStreamOpen())
                rta.closeStream();
            rta.openStream(&params,NULL,RTAUDIO_FLOAT32,sampling_rate,&bufflen,callback,reinterpret_cast<void*>(syn));
        }

        std::vector<std::string> getDevices();
        static int callback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData);
    private:
        uint32_t bufflen;
        RtAudio::StreamParameters params;
        RtAudio rta;
    };
}
#else
namespace sb {
    class RtAudioBackend : public EmitterBackend {
    public:
        explicit RtAudioBackend(sb::Synth*, size_t&, std::map<size_t,bool>&, size_t) {}

        static bool instantiable();
        size_t returnSuggestedBufferSize() {return 0;}
        ~RtAudioBackend() {}
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

#endif // RTAUDIO_H
