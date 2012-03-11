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

#ifndef BACKEND_PORTAUDIO_H
#define BACKEND_PORTAUDIO_H

#include <portaudio.h>

#include "backend.h"

//NOTICE: Do not reimplement this class to use Pa_WriteStream. It uses the callback to take advantage of the high-priority thread it runs in.

class portaudio_backend : public em_backend {
public:
    static bool instantiable();
    explicit portaudio_backend(sb::Synth*, size_t&,std::map<size_t,bool>&,size_t);
    size_t returnSuggestedBufferSize();
    ~portaudio_backend();
    void start();
    void stop();
    void setSamplingRate(size_t);
    static int callback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);

private:
    bool dead;
    PaStream* river;
    PaStreamParameters dev;

};

#endif // BACKEND_PORTAUDIO_H