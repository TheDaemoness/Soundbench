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

#ifndef BACKEND_PULSEAUDIO_H
#define BACKEND_PULSEAUDIO_H

#include <thread>

#include <pulse/simple.h>

#include "backend.h"

//NOTICE: The PulseAudio backend is highly experimental. Attempts that have been made to make it stable have failed.
//If you think that you can make this backend usable in case the PortAudio and JACK backends both fail, then please send us a copy.
//You may get your name in this program's "About" section.

class pulseaudio_backend : public em_backend {
public:
    static bool instantiable();
    explicit pulseaudio_backend(sb::Synth*, size_t&,std::map<size_t,bool>&,size_t);
    size_t returnSuggestedBufferSize();
    ~pulseaudio_backend();
    void start();
    void stop();
    void setSamplingRate(size_t);
    static int executable(pa_simple*,bool*,sb::Synth*,size_t);
private:

    pa_sample_spec* specs;
    pa_channel_map mappi;
    pa_simple* river;
    std::thread* tread;
    bool* run;
#ifdef _WIN32
    const static pa_sample_format sample_format = PA_SAMPLE_FLOAT32LE;
#else
    const static pa_sample_format sample_format = PA_SAMPLE_FLOAT32BE;
#endif
};

#endif // BACKEND_PULSEAUDIO_H
