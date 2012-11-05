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


#include "synth.h"

namespace sb {
    void Synth::tick(SbSample* frames) {
        for (size_t ing = 0; ing < InternalChannels; ++ing) {
            channel_tick(ing,buffer[ing]); //NOOOOO!
            for (size_t acid = 0; acid < OutChannels; ++acid) { //Loop per outbound channel.
                if(!ing) //Only reset the value of frames once.
                    frames[acid] = SbSampleZero;
                frames[acid] += buffer[ing][acid]; //Chemistry failure.
            }
        }
        if (inactivechans != InternalChannels) {
            for (size_t out = 0; out < OutChannels; ++out) {
                frames[out] /= (InternalChannels-inactivechans); //Correctly average the signal from the running channels.
                frames[out] *= vol; //Apply the master volume.
            }
        }
    }

    void Synth::channel_tick(uint8_t ic, SbSample *frame) {
        if (gener[ic] != nullptr)
            gener[ic]->tick(frame, OutChannels);
        else {
            for (size_t z = 0; z < OutChannels; ++z)
                frame[z] = SbSampleZero;
        }
        for (size_t ient = 0; ient < FxPerChannel; ++ient) { //Loop per effect.
            if (eff[ic][ient] != nullptr)
                eff[ic][ient]->tick(frame,OutChannels);
        }
    }

    void Synth::interleaved_block(SbSample *frames, size_t framecount) {
        for (size_t i = 0; i < framecount*OutChannels; ++i)
            frames[i] = SbSampleZero;

        auto singleChannelCallback = [&,this](uint8_t chan) {
            SbSample buff[OutChannels];
            for(size_t p = 0; p < framecount*OutChannels; p+=OutChannels)  {
                channel_tick(chan,buff);
                for(size_t et = 0; et < OutChannels; ++et) {
                    frames[p+et] += buff[et];
                }
            }
        };

        switch(tlevel) {
        case ThreadingChannelwise:
            std::thread* t[InternalChannels];
            for (uint8_t ie = 0; ie < InternalChannels; ++ie) {
                if (gener[ie] != nullptr)
                    t[ie] = new std::thread(singleChannelCallback,ie);
                else
                    t[ie] = nullptr;
            }
            for (uint8_t i = 0; i < InternalChannels; ++i) {
                if (t[i] != nullptr) {
                    t[i]->join();
                    delete t[i];
                }
            }
            break;
        case ThreadingPartial:
        case ThreadingNone:
            for(uint8_t i = 0; i < InternalChannels; ++i) {
                SbSample buff[OutChannels];
                for(size_t onn = 0; onn < framecount*OutChannels; onn+=OutChannels)  {
                    channel_tick(i,buff);
                    for(size_t et = 0; et < OutChannels; ++et)
                        frames[onn+et] += buff[et];
                }
            }
            break;
        }
        if (inactivechans != InternalChannels) {
            for (size_t out = 0; out < OutChannels; ++out) {
                frames[out] /= (InternalChannels-inactivechans); //Correctly average the signal from the running channels.
                frames[out] *= vol; //Apply the master volume.
            }
        }
    }
    void Synth::uninterleaved_blocks(SbSample *lframes, SbSample *rframes, size_t framecount) {
        for (size_t i = 0; i < framecount; ++i) {
            lframes[i] = SbSampleZero;
            rframes[i] = SbSampleZero;
        }

        auto singleChannelCallback = [&,this](uint8_t chan) {
            SbSample buff[2];
            for(size_t es = 0; es < framecount; ++es)  {
                channel_tick(chan,buff);
                lframes[es] += buff[0];
                rframes[es] += buff[1];
            }
        };

        switch(tlevel) {
        case ThreadingChannelwise:
            std::thread* t[InternalChannels];
            for (uint8_t ie = 0; ie < InternalChannels; ++ie) {
                if (gener[ie] != nullptr)
                    t[ie] = new std::thread(singleChannelCallback,ie);
                else
                    t[ie] = nullptr;
            }
            for (uint8_t i = 0; i < InternalChannels; ++i) {
                if (t[i] != nullptr) {
                    t[i]->join();
                    delete t[i];
                }
            }
            break;
        case ThreadingPartial:
        case ThreadingNone:
            for(uint8_t er = 0; er < InternalChannels; ++er) {
                SbSample buff[2];
                for(size_t ong = 0; ong < framecount; ++ong)  {
                    channel_tick(er,buff);
                    lframes[ong] += buff[0];
                    rframes[ong] += buff[1];
                }
            }
            break;
        }
        if (inactivechans != InternalChannels) {
            for (size_t out = 0; out < OutChannels; ++out) {
                lframes[out] /= (InternalChannels-inactivechans); //Correctly average the signal from the running channels.
                lframes[out] *= vol; //Apply the master volume.
                rframes[out] /= (InternalChannels-inactivechans); //Correctly average the signal from the running channels.
                rframes[out] *= vol; //Apply the master volume.
            }
        }
    }
}
