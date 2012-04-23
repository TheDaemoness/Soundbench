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

#ifndef UTILITIES_H
#define UTILITIES_H

/*
    This file has been split off from and included in common.h.

    Should the MIDI file parser prove to be reliable, it may be released as a separate subproject.

    Utilities that *both* Soundbench and the midifile set of files might need will be put here.
    Anything that only Soundbench will need will be found in common.h
    Anything that only the subproject will need will be found in midifile.h

    If you are a project developer, DO NOT ADD ANYTHING TO THIS FILE WITHOUT CONSULTING THE PROJECT HEAD!
*/

#include <stdint.h>
#include <sstream>

namespace sb {

    enum simple_bitmasks { //Notice: These values are for use on single bytes and assume big endian format.
        Bit8 = 1,
        Bit7 = 2,
        Bit6 = 4,
        Bit5 = 8,
        Bit4 = 16,
        Bit3 = 32,
        Bit2 = 64,
        Bit1 = 128,
        NotBit1 = 127, //Put in here because it finds itself used frequently enough.
        AllBits = 255
    };

    namespace midi {
        enum simple_bytemasks { //The first byte is the least significant for the purposes of supporting many different integer sizes, up to uint32_t.
            Byte1 = 0xFF,
            Byte2 = 0xFF00,
            Byte3 = 0xFF0000,
            Byte4 = 0xFF000000
        };

        enum MidiControllers {
            DataSlider = 0x06, //Implement in v. 0.6.0.
            Volume = 0x07,
            Pan = 0x0A,
            ADSR_A = 0x10, //Implement in v. 0.8.0.
            ADSR_D = 0x11, //Implement in v. 0.8.0.
            ADSR_S = 0x12, //Implement in v. 0.8.0.
            ADSR_R = 0x13, //Implement in v. 0.8.0.
            HoldPedal = 0x40,
            SustPedal = 0x42,
            SoftPedal = 0x43, //Implement in v. 0.8.0.
            LegatoPedal = 0x44, //Implement in v. 0.8.0.
            ExtendRelease = 0x45, //Implement in v. 0.8.0.
            DataInc = 0x60, //Implement in v. 0.6.0.
            DataDec = 0x61, //Implement in v. 0.6.0.
            UnregParamNumLSB = 0x62, //Implement in v. 0.6.0.
            UnregParamNumMSB = 0x63, //Implement in v. 0.6.0.
            RegedParamNumLSB = 0x64, //Implement in v. 0.7.0.
            RegedParamNumMSB = 0x65 //Implement in v. 0.7.0.
        };
        enum MidiEvents {
            NoteOff = 0x8,
            NoteOn = 0x9,
            Aftertouch = 0xA,
            Controller = 0xB,
            ProgramChange = 0xC,
            ChannelPressure = 0xD,
            PitchBend = 0xE,
            SystemEvent = 0xF,

            //The following (as far as the developers know) are not used in the MIDI specification. They may be used for internal use.
            Failed = 0x0,
            EndOfTrack = 0x1
        };
        enum MidiSystemEvents {
            SysExEvent = 0xF0,
            QuarterFrameEvent = 0xF1,
            SoundPositionEvent = 0xF2,
            SongSelectEvent = 0xF3,
            TuneRequestEvent = 0xF6,
            MidiClockEvent = 0xF8,
            MidiTickEvent = 0xF9,
            MidiStartEvent = 0xFA,
            MidiStopEvent = 0xFC,
            MidiContinueEvent = 0xFB,
            ActiveSenseMessage = 0xFE,
            MetaEvent = 0xFF
        };
        enum MidiMetaEvents {
            MetaTrackName = 0x02,
            MetaEndOfTrack = 0x2F
        };
    }

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

}

#endif // UTILITIES_H
