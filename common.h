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

#ifndef SOUNDBENCH_API_H
#define SOUNDBENCH_API_H

#include <stdint.h>
#include <utility>
#include <vector>
#include <array>
#include <map>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

#define SBVERSION "Main Development Branch"

typedef float sbSample;
const sbSample sbSampleMin = -1.0;
const sbSample sbSampleMax = 1.0;
const sbSample sbSampleZero = 0.0;

class sbError {
public:
    explicit sbError(std::string module, std::string message,bool iswarning = false) {
        isSevere = !iswarning;
        where = module;
        what = message;
    }
    std::string module() {return where;}
    std::string message() {return what;}
private:
    bool isSevere;
    std::string where;
    std::string what;
};

template <typename outType, typename inType>
outType lexical_cast(inType in) {
    std::stringstream strm;
    strm.str(std::string());
    strm << in;
    outType out;
    strm >> out;
    return out;
}

template <typename inType> //We know that function template specializations are evil.
std::string lexical_cast(inType in) {
    std::stringstream strm;
    strm.str(std::string());
    strm << in;
    return strm.str();
}

namespace sb {
    union ParameterValue { //For all you C++ programmers out there who don't know what this is... it's a C-style space-saver. ;)
        sbSample sample;
        size_t value;
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
        valu.value = static_cast<size_t>(i); //Do NOT make this a dynamic_cast.
        return valu;
    }
    inline ParameterValue makeParamfromSample(sbSample i) {
        ParameterValue valu;
        valu.sample = i;
        return valu;
    }

    enum simple_bitmasks {
        Bit1 = 1,
        Bit2 = 2,
        Bit3 = 4,
        Bit4 = 8,
        Bit5 = 16,
        Bit6 = 32,
        Bit7 = 64,
        Bit8 = 128,
        NotBit8 = 127
    };

    enum channel_mapping {
        Mono,
        Stereo,
        StereoLeft,
        StereoRight
    };

    enum emitter_type {
        NoEmitter = 0,
        PortAudio,
        JACK_O //Not implemented.
    };

    enum midi_type {
        NoMIDI = 0,
        PortMIDI, //Not implemented. Will be implemented in 0.3.0
        JACK_I //Not implemented.
    };

    enum SimpleWaveTypes {
        SineWave, TriangleWave, SquareWave, SawtoothWave
    };

    extern size_t curr_srate;
    const size_t outchans = 2;
    const float pi = 3.14159265358979323846264338327950288f;

    const size_t default_poly = 16;
    const unsigned char channelcount = 4;
    const unsigned char fxcount = 4;

    const size_t sampling_rates[] = {44100,48000,88200,96000,176400,192000};
    const size_t sampling_rate_count = 6;

    inline float getFrequencyFromNote(unsigned char note, float A4 = 440.00) {
        return A4*std::pow(1.059463094359,note-48);
    }
}

#endif // SOUNDBENCH_API_H
