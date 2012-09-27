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

#ifndef EMITTER_H
#define EMITTER_H


//local
#include "sbutilities.h"
#include "backend/portaudio.h"
#include "backend/rtaudio.h"
#include "backend/jackaudio.h"

//libstdc++11
#include <vector>

class Synth;

namespace sb {

    class Emitter {
    public:
        explicit Emitter(Synth*);
        ~Emitter();

        inline size_t getSampleRate() {
            return sample_rate;
        }
        inline std::map<size_t,bool> getSupportedRates() {
            return supported_rates;
        }
        inline std::map<EmitterType,bool> getSupportedAPIs() {
            return supported_apis;
        }
        inline std::vector<std::string> getDevices() {
            if(backend != nullptr)
                return backend->getDevices();
            return std::vector<std::string>();
        }
        inline bool isRtAvailable() {
            return backend != nullptr;
        }

        void setSamplingRate(size_t);
        void setEmitterType(EmitterType);


        size_t getCurrentDevice() {
            if (backend != nullptr)
                return backend->getCurrentDevice();
            return 0;
        }

        size_t getDefaultDevice() {
            if (backend != nullptr)
                return backend->getDefaultDevice();
            return 0;
        }

        void start() {
            if (backend != nullptr)
                backend->start();
        }
        void stop() {
            if (backend != nullptr)
                backend->stop();
        }
        bool isRunning() {
            if (backend != nullptr)
                return backend->isRunning();
            return false;
        }

    private:
        bool initSomeBackend(EmitterType notthisone = NoEmitter);
        bool initPortAudio();
        bool initRtAudio();

        EmitterType em_type, used_backend;
        std::map<EmitterType,bool> supported_apis;
        std::map<size_t,bool> supported_rates;
        EmitterBackend* backend;
        size_t sample_rate;
        Synth* syn;
    };

}

#endif // EMITTER_H
