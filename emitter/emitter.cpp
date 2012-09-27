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

    Emitter::Emitter(Synth* s) {
#ifdef NO_AUDIOBACKEND
        std::cerr << "Soundbench was compiled without any audio backends.\n";
        em_type = NoEmitter;
#else
        std::cerr << "Determining which audio backends will initialize...\n";
        supported_apis[PortAudio_O] = PortaudioBackend::instantiable();
        supported_apis[RtAudio_O] = RtAudioBackend::instantiable();
        supported_apis[JACK_O] = JackAudioBackend::instantiable();

        if (supported_apis[PortAudio_O])
            em_type = PortAudio_O;
        else if (supported_apis[JACK_O])
            em_type = JACK_O;
        else if (supported_apis[RtAudio_O])
            em_type = RtAudio_O;
        else {
            std::cerr << "...none will initialize.\n";
            ErrorPopup* er = new ErrorPopup;
            er->setErrorText("All Backends Unusable");
            er->setInfoText("None of the audio backends report that they can be initialized. This is not necessarily due to a fault in Soundbench.\n\nThis error may be ignored, but the program would run without real-time audio output.");

            errs::fixes::Ignore* ign = new errs::fixes::Ignore;
            ign->setComments("Run Soundbench without realtime audio support.\nSoundbench will not be able to output any sound except by writing it to a file.");
            er->addFix(ign);
            er->exec();
            bool killoption = !er->wasFixed();
            delete er;
            if (killoption)
                AbortSoundbench();
            em_type = NoEmitter;
        }

        syn = s;
        sample_rate = SampleRate;
        backend = nullptr;
        setEmitterType(em_type);
    }

    void Emitter::setEmitterType(EmitterType emt) {
        if (backend != nullptr) {
            delete backend;
            backend = nullptr;
        }
        if (emt == NoEmitter)
            return;
        used_backend = NoEmitter;
        bool initialed = false;

        if (emt == JACK_O) {
            used_backend = JACK_O;
            std::cerr << "Would initialize a JACK as an audio backend, but it hasn't yet been implemented.\n";
            initSomeBackend(JACK_O);
        }
        else if (emt == RtAudio_O)
            initialed = initBackend<RtAudio_O,RtAudioBackend>(this);
        else if (emt == PortAudio_O)
            initialed = initBackend<PortAudio_O,PortaudioBackend>(this);

        //Try the other backends if the chosen one didn't work out.
        if (!initialed)
            initSomeBackend(emt);

        if (!initialed && backend != nullptr) {
            //A different backend is in use.
            WarningPopup* w = new WarningPopup;
            w->setWarningText("Alternate Backend in Use");
            w->setInfoText("Soundbench could not initialize the desired audio backend and is using a backend different from the one requested.");
            w->exec();
            bool noquite = w->fixed();
            delete w;
            if (!noquite) {
                delete backend;
                backend = nullptr;
                return;
            }
            else
                AbortSoundbench();
        }
        else if (!initialed && backend == nullptr) {
            //All the backends failed.
            ErrorPopup* er = new ErrorPopup;
            er->setErrorText("All Backends Failed");
            er->setInfoText("All the audio backends failed to initialize. This is not necessarily due to a fault in Soundbench.\n\nThis error may be ignored, but the program would run without real-time audio output.");

            errs::fixes::Ignore* ign = new errs::fixes::Ignore;
            ign->setComments("Run Soundbench without realtime audio support.\nSoundbench will not be able to output any sound except by writing it to a file.");
            er->addFix(ign);
            er->exec();
            bool throw22 = !er->wasFixed();
            delete er;
            if (throw22)
                AbortSoundbench();
        }
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
