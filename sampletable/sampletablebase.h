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


#ifndef SAMPLETABLEBASE_H
#define SAMPLETABLEBASE_H

#include "sbutilities.h"

namespace sb {
    class SampleTableBase {
    public:
        virtual SbSample tick(size_t pos = 0) = 0;

        virtual void setIterator(float freq, SbSample amp, size_t pos) = 0;
        virtual void setIteratorOffset(size_t pos, size_t offset = 0) = 0;

        virtual size_t getIteratorCount() = 0;
        virtual void setIteratorCount(size_t count) = 0;

        virtual void reset() {
            for(size_t i = 0; i < getIteratorCount(); ++i)
                resetIterator(i);
        }

        virtual SbSample getSample(size_t loc) = 0;

        virtual void setAllIteratorOffsets(size_t offset = 0) {
            for(size_t i = 0; i < getIteratorCount(); ++i)
                setIteratorOffset(i,offset);
        }
        virtual void resetIterator(size_t pos) {
            setIterator(0.0F,SbSampleZero,pos);
            setIteratorOffset(pos);
        }
    };

}

#endif // SAMPLETABLEBASE_H
