#ifndef SYNTHBASE_H
#define SYNTHBASE_H

#include "soundbases.h"

namespace sb {

    enum ThreadLevel {
        ThreadingNone = 0,
        ThreadingChannelwise = 1,
        ThreadingPartial = 2
    };

    /*
        WARNING: This enum is *very* close to conflicting with the one on midienums.h.

        Although they are in different namespaces, for the sake of avoiding confusion, please try to keep the names in each one different.
    */
    enum SupportedPedals {
        NoPedal,
        Hold,
        Sustenuto
    };

    class SynthBase {
    public:
        virtual void noteOn(int halfsteps, SbSample amp) = 0;
        virtual void noteOff(int halfsteps) = 0;
        virtual void pedal(SupportedPedals which, bool val = true) = 0;
        virtual void reset() = 0;
        virtual void tick(SbSample* frame) = 0;
        virtual void channel_tick(uint8_t chanval, SbSample* frame) = 0;
        virtual void interleaved_block(SbSample* frames, size_t framecount) = 0;
        virtual void uninterleaved_blocks(SbSample* lframes, SbSample* rframes, size_t framecount) = 0;
        virtual uint8_t channel_count() = 0;

        virtual void setPolyphony(uint8_t) = 0;
        virtual void updateSamplingRate() = 0;

        inline SbSample& volume() {return vol;}
        inline ThreadLevel getThreadLevel() {return tlevel;}

    protected:
        ThreadLevel tlevel;
        SbSample vol;
    };

}

#endif // SYNTHBASE_H
