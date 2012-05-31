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
#include "backend.portaudio.h"

//libstdc++11
#include <vector>

class Synth;

namespace sb {

    class Emitter {
    public:
        explicit Emitter(Synth*);
        ~Emitter();

        bool failed() {
            return failure;
        }

        size_t getSampleRate() {
            return sample_rate;
        }
        std::map<size_t,bool> getSupportedRates() {
            return supported_rates;
        }
        std::map<emitter_type,bool> getSupportedAPIs() {
            return supported_apis;
        }

        void setSamplingRate(size_t);
        void setEmitterType(emitter_type);

        void start() {
            backend->start();
        }
        void stop() {
            backend->stop();
        }
        bool isRunning() {
            return backend->isRunning();
        }

    private:
        bool failure;
        emitter_type em_type;
        std::map<emitter_type,bool> supported_apis;
        std::map<size_t,bool> supported_rates;
        em_backend* backend;
        size_t sample_rate;
        Synth* syn;
    };

}

#endif // EMITTER_H
