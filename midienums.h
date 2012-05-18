#ifndef MIDIUTILS_H
#define MIDIUTILS_H

namespace sb {
    namespace midi {

        enum MidiControllers {
            BankSelectMSB = 0x00,
            ModulationMSB = 0x01,
            BreathControllerMSB = 0x02,
            FootControllerMSB = 0x04,
            PortamentoTimeMSB = 0x05,
            DataSliderMSB = 0x06,
            VolumeMSB = 0x07,
            BalanceMSB = 0x08,
            PanMSB = 0x0A,
            ExpressionControllerMSB = 0x0B,
            EffectControl1MSB = 0x0C,
            EffectControl2MSB = 0x0D,
#ifdef IS_SOUNDBENCH
            ADSR_A_MSB = 0x10,
            ADSR_D_MSB = 0x11,
            ADSR_S_MSB = 0x12,
            ADSR_R_MSB = 0x13,
#else
            GeneralPurpose1MSB = 0x10,
            GeneralPurpose2MSB = 0x11,
            GeneralPurpose3MSB = 0x12,
            GeneralPurpose4MSB = 0x13,
#endif
            BankSelectLSB = 0x20,
            ModulationLSB = 0x21,
            BreathControllerLSB = 0x22,
            FootControllerLSB = 0x24,
            PortamentoTimeLSB = 0x25,
            DataSliderLSB = 0x26,
            VolumeLSB = 0x27,
            BalanceLSB = 0x28,
            PanLSB = 0x0A,
            ExpressionControllerLSB = 0x2B,
            EffectControl1LSB = 0x2C,
            EffectControl2LSB = 0x2D,
#ifdef IS_SOUNDBENCH
            ADSR_A_LSB = 0x30,
            ADSR_D_LSB = 0x31,
            ADSR_S_LSB = 0x32,
            ADSR_R_LSB = 0x33,
#else
            GeneralPurpose1LSB = 0x30,
            GeneralPurpose2LSB = 0x31,
            GeneralPurpose3LSB = 0x32,
            GeneralPurpose4LSB = 0x33,
#endif
            HoldPedal = 0x40,
            Portamento = 0x41,
            SustenutoPedal = 0x42,
            SoftPedal = 0x43,
            LegatoPedal = 0x44,
            ExtendRelease = 0x45,
            SoundController01 = 0x46,
            SoundController02 = 0x47,
            SoundController03 = 0x48,
            SoundController04 = 0x49,
            SoundController05 = 0x4A,
            SoundController06 = 0x4B,
            SoundController07 = 0x4C,
            SoundController08 = 0x4D,
            SoundController09 = 0x4E,
            SoundController10 = 0x4F,
#ifdef IS_SOUNDBENCH
            //Someday a purpose may be found for those four controllers.
#else
            GeneralPurpose5 = 0x50,
            GeneralPurpose6 = 0x51,
            GeneralPurpose7 = 0x52,
            GeneralPurpose8 = 0x53,
#endif
#ifdef IS_SOUNDBENCH
            MasterChannelFxLevel = 0x5B,
            Channel1FxLevel = 0x5C,
            Channel2FxLevel = 0x5D,
            Channel3FxLevel = 0x5E,
            Channel4FxLevel = 0x5F,
#else
            Effects1Depth = 0x5B,
            Effects2Depth = 0x5C,
            Effects3Depth = 0x5D,
            Effects4Depth = 0x5E,
            Effects5Depth = 0x5F,
#endif
            DataInc = 0x60,
            DataDec = 0x61,
            UnregParamNumLSB = 0x62,
            UnregParamNumMSB = 0x63,
            RegedParamNumLSB = 0x64,
            RegedParamNumMSB = 0x65,
            AllSoundOff = 0x78,
            AllControllersOff = 0x79,
            ToggleLocalKeyboard = 0x7A,
            AllNotesOff = 0x7B,
            OmniModeOff = 0x7C,
            OmniModeOn = 0x7D,
            MonophonicMode = 0x7E,
            PolyphonicMode = 0x7F
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
            NoDataRead = 0x0,
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
            MetaTrackName = 0x03,
            MetaEndOfTrack = 0x2F
        };
        enum MidiFileTypes {
            SingleTrack = 0,
            MultiTrack = 1,
            MultiSong = 2
        };
    }
}

#endif // MIDIUTILS_H
