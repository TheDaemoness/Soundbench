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

#ifndef SBUTILITIES_H
#define SBUTILITIES_H

/*
  This file is for anything that a large number of files in Soundbench might need.

  If you are a project developer, DO NOT ADD ANYTHING TO THIS FILE WITHOUT CONSULTING THE PROJECT HEAD!
*/

#include <utility>
#include <map>
#include <string>
#include <cmath>

#include "common.h"
#include "problemfix.h"

#ifndef IS_SOUNDBENCH
#define IS_SOUNDBENCH
#endif

#if defined(_POSIX_VERSION) | defined(__CYGWIN__)
#define SB_ENV_POSIX
#elif defined(_WIN32)
#define SB_ENX_WNDOS
#endif

#define SBVERSION "Development Branch"

typedef float SbSample;
const SbSample SbSampleMin = -1.0;
const SbSample SbSampleMax = 1.0;
const SbSample SbSampleZero = 0.0;

class sbException {
public:
    sbException(std::string type, std::string info = "",
            sb::errs::fixes::ProblemFix* fix1 = nullptr,
            sb::errs::fixes::ProblemFix* fix2 = nullptr,
            sb::errs::fixes::ProblemFix* fix3 = nullptr,
            sb::errs::fixes::ProblemFix* fix4 = nullptr,
            sb::errs::fixes::ProblemFix* fix5 = nullptr,
            sb::errs::fixes::ProblemFix* fix6 = nullptr,
            sb::errs::fixes::ProblemFix* fix7 = nullptr) {
        errdata.first = type;
        errdata.second = info;
        fixlist.push_back(fix1);
        fixlist.push_back(fix2);
        fixlist.push_back(fix3);
        fixlist.push_back(fix4);
        fixlist.push_back(fix5);
        fixlist.push_back(fix6);
        fixlist.push_back(fix7);
    }
    std::pair<std::string, std::string> errdata;
    std::vector<sb::errs::fixes::ProblemFix*> fixlist;
};

namespace sb {
    union ParameterValue { //For all you C++ programmers out there who don't know what this is... it's a C-style space-saver. ;)
        SbSample sample;
        int64_t value;
        void* other;
    };
    template <typename T>
    inline ParameterValue makeParamfromPointer(T* ptr) {
        ParameterValue valu;
        valu.other = reinterpret_cast<void*>(ptr);
        return valu;
    }
    template <typename T>
    inline ParameterValue makeParamfromInt(T i) {
        ParameterValue valu;
        valu.value = static_cast<int64_t>(i);
        return valu;
    }
    inline ParameterValue makeParamfromSample(SbSample i) {
        ParameterValue valu;
        valu.sample = i;
        return valu;
    }

    enum ChannelMapping {
        Mono,
        Stereo,
        StereoLeft,
        StereoRight
    };

    enum EmitterType {
        NoEmitter,
        PortAudio,
        JACK_O //Not implemented.
    };

    enum FrontendType {
        NoMIDI,
        PortMIDI,
        JACK_I //Not implemented.
    };

    enum SimpleWaveTypes {
        SineWave = 1,
        TriangleWave = 2,
        SquareWave = 3,
        SawtoothWave = 4,
        OvalWave = 5
    };

    extern size_t SampleRate;
    const size_t OutChannels = 2;
    const float Pi = 3.14159265358979323846264338327950288f;

    const size_t DefaultPolyphony = 16;
    const unsigned char InternalChannels = 4;
    const unsigned char FxPerChannel = 4;

    const size_t SupportedRates[] = {44100,48000,88200,96000,176400,192000};
    const size_t SupportedRatesCount = 6;

    inline float getFrequencyFromNote(int8_t delta, float A4 = 440.00) {
        return A4*std::pow(pow(2.0,1.0/12),delta);
    }
}

#endif // COMMON_H
