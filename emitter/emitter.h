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
        template <sb::EmitterType TypeEnum, class Backside>
        friend bool initBackend(Emitter* b);

    public:
        explicit Emitter(Synth*);
        ~Emitter();

        void stopAndUpdateSampleRate() {
            if (backend != nullptr) {
                backend->stop();
                backend->setSamplingRate(global_srate);
                std::cerr << "Updated the sampling rate to " << global_srate << ".\n";
            }
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

        void setEmitterType(EmitterType);

        bool doesBackendUsePorts() {
            if (backend != nullptr)
                return backend->usesPorts();
            return false;
        }
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
        inline EmitterType getCurrentAPI() {
            return em_type;
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

        EmitterType em_type;
        std::map<EmitterType,bool> supported_apis;
        std::map<size_t,bool> supported_rates;
        EmitterBackend* backend;
        Synth* syn;
    };

}

#endif // EMITTER_H
