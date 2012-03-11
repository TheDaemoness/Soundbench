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

#include "backend.portaudio.h"

#include <chrono>

portaudio_backend::portaudio_backend(sb::Synth* s, size_t& srate, std::map<size_t, bool>& srates, size_t chans) {
    int e;
    e = Pa_Initialize();
    if (e != paNoError) {
        throw sbError("emitter::backend::portaudio",Pa_GetErrorText(e));
    }
    dev.device = Pa_GetDefaultOutputDevice();
    if (dev.device == paNoDevice) {
        throw sbError("emitter::backend::portaudio","No default device");
    }

    dev.channelCount = chans;
    dev.sampleFormat = paFloat32;
    dev.suggestedLatency = Pa_GetDeviceInfo(dev.device)->defaultLowOutputLatency;
    dev.hostApiSpecificStreamInfo = NULL;

    for(size_t i = 0; i < sb::sampling_rate_count; ++i)
        srates[sb::sampling_rates[i]] = (Pa_IsFormatSupported(NULL,&dev,sb::sampling_rates[i]) == paFormatIsSupported);

    if (!srates[sb::sampling_rates[1]]) {
        for (size_t i = 0; i < sb::sampling_rate_count; ++i) {
            if(srates[sb::sampling_rates[i]])
                srate = sb::sampling_rates[i];
        }
    }
    sampling_rate = srate;
    syn = s;
    river = NULL;
}

int portaudio_backend::callback(const void*, void* output, unsigned long framecount, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* syne) {
    sbSample* framepointer = reinterpret_cast<sbSample*>(output);
    for (size_t i = 0; i < framecount; framepointer += sb::outchans, ++i)
        reinterpret_cast<sb::Synth*>(syne)->tick(framepointer,sb::outchans);
    return paContinue;
}

void portaudio_backend::start() {
    int e = paNoError;
    if (river == NULL)
        e = Pa_OpenStream(&river,NULL,&dev,sampling_rate,paFramesPerBufferUnspecified,paNoFlag,portaudio_backend::callback,reinterpret_cast<void*>(syn));
    if (e != paNoError) {
        std::cerr << "Problem when opening the output stream: " << Pa_GetErrorText(e) << ".\n";
        throw sbError("emitter::backend::portaudio",std::string("Couldn't open the output stream: ")+Pa_GetErrorText(e));
    }
    e = Pa_StartStream(river);
    if (e != paNoError) {
        std::cerr << "Problem when starting the output stream: " << Pa_GetErrorText(e) << ".\n";
        throw sbError("emitter::backend::portaudio",std::string("Couldn't start the output stream: ")+Pa_GetErrorText(e));
    }
}

void portaudio_backend::setSamplingRate(size_t neorate) {
    stop();
    sampling_rate = neorate;
}

void portaudio_backend::stop() {
    int e;
    if(river != NULL) {
        if (Pa_IsStreamActive(river)) {
            e = Pa_StopStream(river);
            if (e != paNoError) {
                std::cerr << "Problem when stopping the output stream: " << Pa_GetErrorText(e) << ".\n";
                throw sbError("emitter::backend::portaudio",std::string("Couldn't stop the output stream: ")+Pa_GetErrorText(e));
            }
        }
        e = Pa_CloseStream(river);
        if (e != paNoError) {
            std::cerr << "Problem when closing the output stream: " << Pa_GetErrorText(e) << ".\n";
            throw sbError("emitter::backend::portaudio",std::string("Couldn't close the output stream: ")+Pa_GetErrorText(e));
        }
    }
    river = NULL;
}

size_t portaudio_backend::returnSuggestedBufferSize() {
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

portaudio_backend::~portaudio_backend() {
    stop();
    Pa_Terminate();
}

bool portaudio_backend::instantiable() {
    return true; //I know. I know. This isn't quite fair.
}
