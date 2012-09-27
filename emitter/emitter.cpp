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

#include "emitter.h"
#include "warningpopup.h"
#include "errorpopup.h"
#include "initbackend.h"

namespace sb {

    void Emitter::setEmitterType(EmitterType emt) {
        if (backend != nullptr) {
            delete backend;
            backend = nullptr;
        }
        if (emt == NoEmitter)
            return;
        used_backend = NoEmitter;

        if (emt == JACK_O) {
            used_backend = JACK_O;
            std::cerr << "Would initialize a JACK as an audio backend, but it hasn't yet been implemented.\n";
            initSomeBackend(JACK_O);
        }
        else if (emt == RtAudio_O) {
            if(!initBackend<RtAudio_O,RtAudioBackend>(this))
                initSomeBackend(RtAudio_O);
        }
        else if (emt == PortAudio_O) {
            if(!initBackend<PortAudio_O,PortaudioBackend>(this))
                initSomeBackend(PortAudio_O);
        }

        if (backend != nullptr && used_backend != emt) {
            //A different backend is in use.
            WarningPopup* w = new WarningPopup;
            w->setWarningText("Alternate Backend in Use");
            w->setInfoText("Soundbench could not initialize the desired audio backend and is using an backend different from the one requested.");
            w->exec();
            bool noquite = w->fixed();
            delete w;
            if (!noquite) {
                delete backend;
                backend = nullptr;
                return;
            }
        }
        else if (backend == nullptr && emt != NoEmitter && used_backend == NoEmitter) {
            //A completely unknown backend was requested.
            std::__throw_logic_error("A completely unknown audio backend was requested.\nTHIS IS A SEVERE INTERAL ERROR so if you see this in an official binary, please submit it as a bug report.");
        }
        else if (backend == nullptr && emt != NoEmitter) {
            //All the backends failed.
            ErrorPopup* er = new ErrorPopup;
            er->setErrorText("All Audio Backends Failed");
            er->setInfoText("All the audio backends failed to initialize. This is not necessarily Soundbench's problem, but may be that of your operating system.\n\nThis error may be ignored, but the program would run without real-time audio output.");

            errs::fixes::Ignore ign;
            ign.setComments("Run Soundbench without realtime audio support.\nSoundbench will not be able to output any sound save by writing it to a file.");
            er->addFix(&ign);
            delete er;
        }
    }

    Emitter::Emitter(Synth* s) {
        std::cerr << "Determining which audio backends will initialize...\n";
        supported_apis[PortAudio_O] = PortaudioBackend::instantiable();
        supported_apis[RtAudio_O] = RtAudioBackend::instantiable();
        supported_apis[JACK_O] = JackAudioBackend::instantiable();
        syn = s;

        if (supported_apis[PortAudio_O])
            em_type = PortAudio_O;
        else if (supported_apis[JACK_O])
            em_type = JACK_O;
        else if (supported_apis[RtAudio_O])
            em_type = RtAudio_O;
        else {
            std::cerr << "No real time audio backends can be initialized on this computer.\n";
            em_type = NoEmitter;
        }

        sample_rate = SampleRate;
        backend = nullptr;
        setEmitterType(em_type);
    }

    void Emitter::setSamplingRate(size_t s_rate) {
        SampleRate = s_rate;
        if (backend != nullptr)
            backend->setSamplingRate(s_rate);
        std::cerr << "Set sampling rate to " << SampleRate << " samples per second.\n";
    }

    Emitter::~Emitter() {
        if (backend != nullptr)
            delete backend;
    }

    bool Emitter::initSomeBackend(EmitterType notthisone) {
        for (auto pair : supported_apis) {
            if (pair.first == PortAudio_O && notthisone != PortAudio_O) {
                if (initBackend<PortAudio_O,PortaudioBackend>(this))
                    return true;
            }
            else if (pair.first == RtAudio_O && notthisone != RtAudio_O) {
                if (initBackend<RtAudio_O,RtAudioBackend>(this))
                    return true;
            }
        }
        backend = nullptr;
        return false;
    }
}
