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

#include "portaudio.h"

namespace sb {
#ifndef NO_PORTAUDIO

    bool PortaudioBackend::pa_inited = false;
    PortaudioBackend::PortaudioBackend(Synth* s, size_t& srate, std::map<size_t, bool>& srates, size_t chans) {
        running = false;
        ready = false;
        int e;
        if (!pa_inited) {
            e = Pa_Initialize();
            if (e != paNoError) {
                std::cerr << "Unexpected initialization failure.\n";
                return;
            }
        }

        dev.device = Pa_GetDefaultOutputDevice();
        dev.channelCount = chans;
        dev.sampleFormat = paFloat32;
        dev.suggestedLatency = Pa_GetDeviceInfo(dev.device)->defaultLowOutputLatency;
        dev.hostApiSpecificStreamInfo = nullptr;

        for(size_t i = 0; i < SupportedRatesCount; ++i)
            srates[SupportedRates[i]] = (Pa_IsFormatSupported(nullptr,&dev,SupportedRates[i]) == paFormatIsSupported);

        if (!srates[SupportedRates[1]]) {
            std::cerr << "Primary sampling rate not supported.\n";
            return;
        }
        sampling_rate = srate;
        syn = s;
        river = nullptr;
        ready = true;
    }

    int PortaudioBackend::callback(const void*, void* output, unsigned long framecount, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* syne) {
        reinterpret_cast<Synth*>(syne)->interleaved_block(reinterpret_cast<SbSample*>(output),framecount);
        /*
        SbSample* framepointer = reinterpret_cast<SbSample*>(output);
        for (size_t i = 0; i < framecount; framepointer += OutChannels, ++i)
            reinterpret_cast<Synth*>(syne)->tick(framepointer);
        */
        return paContinue;
    }

    void PortaudioBackend::start() {
        int e = paNoError;
        if (river == nullptr) {
            e = Pa_OpenStream(&river,nullptr,&dev,sampling_rate,paFramesPerBufferUnspecified,paNoFlag,PortaudioBackend::callback,reinterpret_cast<void*>(syn));
            if (e != paNoError) {
                std::cerr << "Problem when opening the output stream: " << Pa_GetErrorText(e) << ".\n";
                std::__throw_runtime_error((std::string("emitter::backend::portaudio - Couldn't open the output stream: ")+Pa_GetErrorText(e)).c_str());
            }
        }
        if (!Pa_IsStreamActive(river))
            e = Pa_StartStream(river);
        if (e != paNoError) {
            std::cerr << "Problem when starting the output stream: " << Pa_GetErrorText(e) << ".\n";
            stop();
            running = false;
        }
        else
            running = true;
    }

    void PortaudioBackend::setSamplingRate(size_t neorate) {
        stop();
        sampling_rate = neorate;
    }

    void PortaudioBackend::stop() {
        int e;
        if(river != nullptr) {
            if (Pa_IsStreamActive(river)) {
                e = Pa_StopStream(river);
                if (e != paNoError)
                    std::cerr << "Problem when stopping the output stream: " << Pa_GetErrorText(e) << ".\n";
            }
            e = Pa_CloseStream(river);
            if (e != paNoError)
                std::cerr << "Problem when closing the output stream: " << Pa_GetErrorText(e) << ".\n";
            river = nullptr;
        }
        running = false;
    }

    size_t PortaudioBackend::returnSuggestedBufferSize() {
        PaDeviceIndex dev = Pa_GetDefaultOutputDevice();
        if (dev == paNoDevice)
            return sampling_rate/100;
        const PaDeviceInfo* inf = Pa_GetDeviceInfo(dev);
        if (inf->defaultLowOutputLatency == 0) {
            std::cerr << "Faster-than-light electrons detected in your computer (estimated PortAudio latency = 0).\nResorting to default buffer size...\n";
            return sampling_rate/100;
        }
        return inf->defaultLowOutputLatency*sampling_rate;
    }

    PortaudioBackend::~PortaudioBackend() {
        stop();
        Pa_Terminate();
        pa_inited = false;
    }

    bool PortaudioBackend::instantiable() {
        PaError e = Pa_Initialize();
        if (e != paNoError) {
            std::cerr << "Could not initialize PortAudio: " << Pa_GetErrorText(e) << ". Skipping this backend...\n";
            return false;
        }
        if(Pa_GetDeviceCount() == 0) {
            std::cerr << "Portaudio could not find any available devices. Skipping this backend...\n";
            Pa_Terminate();
            return false;
        }
        if  (Pa_GetDefaultOutputDevice() == paNoDevice) {
            std::cerr << "Portaudio could not determine a default device. Skipping this backend...\n";
            Pa_Terminate();
            return false;
        }
        if (Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice())->maxOutputChannels < 2) {
            std::cerr << "The default device for Portaudio does not support stereo audio. Skipping this backend...\n";
            Pa_Terminate();
            return false;
        }
        pa_inited = true;
        return true;
    }

    std::vector<std::string> PortaudioBackend::getDevices() {
        std::vector<std::string> vecu;
        for(int i = 0; i < Pa_GetDeviceCount(); ++i)
            vecu.push_back(Pa_GetDeviceInfo(i)->name);
        return vecu;
    }

#else
    bool PortaudioBackend::instantiable() {
        return false; //Not compiled PortAudio support.
    }
#endif
} //namespace sb
