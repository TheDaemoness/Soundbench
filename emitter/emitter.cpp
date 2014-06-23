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
#include "err/warningpopup.h"
#include "err/errorpopup.h"
#include "initbackend.h"

namespace sb {

	Emitter::Emitter(Synth* s) {
		for(int i = 0; i < SAMPLING_RATES_COUNT; ++i)
			supported_rates[SAMPLING_RATES[i]] = false;
#ifdef NO_AUDIOBACKEND
		std::cerr << "Soundbench was compiled without any audio backends.\n";
		em_type = NoEmitter;
#else
		std::cerr << "Determining which audio backends will initialize...\n";
		supported_apis[PORTAUDIO_O] = PortaudioBackend::instantiable();
		supported_apis[RTAUDIO_O] = RtAudioBackend::instantiable();
		supported_apis[JACK_O] = JackAudioBackend::instantiable();

		if (supported_apis[JACK_O])
			em_type = JACK_O;
		else if (supported_apis[PORTAUDIO_O])
			em_type = PORTAUDIO_O;
		else if (supported_apis[RTAUDIO_O])
			em_type = RTAUDIO_O;
		else {
			std::cerr << "...none will initialize.\n";
			ErrorPopup* er = new ErrorPopup;
			er->setErrorText("All Backends Unusable");
			er->setInfoText("None of the audio backends report that they can be initialized. This is not necessarily due to a fault in Soundbench.\n\nThis error may be ignored, but the program would run without real-time audio output.");

			errs::fixes::Ignore* ign = new errs::fixes::Ignore;
			ign->setComments("Run Soundbench without realtime audio support.\nSoundbench will not be able to output any sound except by writing it to a file.");
			er->addFix(ign);
			er->exec();
			bool killoption = !er->fixed();
			delete er;
			if (killoption)
				AbortSoundbench();
			em_type = NO_EMITTER;
		}
#endif

		syn = s;
		backend = nullptr;
		setEmitterType(em_type);
	}

	void Emitter::setEmitterType(EmitterType emt) {
		if (backend != nullptr) {
			delete backend;
			backend = nullptr;
		}
		if (emt == NO_EMITTER)
			return;
		em_type = emt;
		bool initialed = false;

		if (emt == PORTAUDIO_O)
			initialed = initBackend<PORTAUDIO_O,PortaudioBackend>(this);
		else if (emt == JACK_O)
			initialed = initBackend<JACK_O,JackAudioBackend>(this);
		else if (emt == RTAUDIO_O)
			initialed = initBackend<RTAUDIO_O,RtAudioBackend>(this);

		//Try the other backends if the chosen one didn't work out.
		if (!initialed)
			initSomeBackend(emt);

		if (backend != nullptr) {
			if(emt == RTAUDIO_O)
				std::cerr << "Using an RtAudio backend.\n";
			else if (emt == PORTAUDIO_O)
				std::cerr << "Using a PortAudio backend.\n";
			else if (emt == JACK_O)
				std::cerr << "Using a JACK backend.\n";
		}

		if (!initialed && backend != nullptr) {
			//A different backend is in use.
			WarningPopup* w = new WarningPopup;
			w->setWarningText("Alternate Backend in Use");
			w->setInfoText("Soundbench could not initialize the desired audio backend and is using a backend different from the one requested.");
			w->exec();
			bool noquite = w->fixed();
			delete w;
			if (noquite) {
				delete backend;
				backend = nullptr;
				return;
			}
			else {
				delete backend;
				AbortSoundbench();
			}
		}
		else if (!initialed && backend == nullptr) {
			//All the backends failed.
			ErrorPopup* er = new ErrorPopup;
			em_type = NO_EMITTER;
			er->setErrorText("All Backends Failed");
			er->setInfoText("All the audio backends failed to initialize. This is not necessarily due to a fault in Soundbench.\n\nThis error may be ignored, but the program would run without real-time audio output.");

			errs::fixes::Ignore* ign = new errs::fixes::Ignore;
			ign->setComments("Run Soundbench without realtime audio support.\nSoundbench will not be able to output any sound except by writing it to a file.");
			er->addFix(ign);
			er->exec();
			bool throw22 = !er->fixed();
			delete er;
			if (throw22)
				AbortSoundbench();
		}
	}
	Emitter::~Emitter() {
		if (backend != nullptr)
			delete backend;
	}

	bool Emitter::initSomeBackend(EmitterType notthisone) {
		for (auto pair : supported_apis) {
			if (pair.first == PORTAUDIO_O && notthisone != PORTAUDIO_O) {
				if (initBackend<PORTAUDIO_O,PortaudioBackend>(this))
					return true;
			}
			else if (pair.first == RTAUDIO_O && notthisone != RTAUDIO_O) {
				if (initBackend<RTAUDIO_O,RtAudioBackend>(this))
					return true;
			}
		}
		backend = nullptr;
		return false;
	}
}
