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

#ifndef MODULES_H
#define MODULES_H

#include "sbutilities.h"
#include "basicwaves.h"

#include <cstring>

namespace sb {

    enum moduleParam {
        genBasic_wave,
        genBasic_amplutide,
        genBasic_phase,
        genBasic_notebias
    };

    class soundBase {
    public:
        soundBase()  {}
        virtual ~soundBase()  {}

        virtual void tick(float* sample, size_t chans) = 0;
        virtual void ctrl(moduleParam arg, ParameterValue val) = 0; //See the documentation for notes about this function.
        virtual void reset() = 0;
    protected:
        size_t sample_rate;
    };


    class genBase : public soundBase {
    public:
        virtual void noteOn(int halfsteps, sbSample amp, size_t pos) = 0;
        virtual void noteOff(size_t pos) = 0;
        virtual void setPolymorphism(size_t) = 0;
    };

    class fxBase : public soundBase {
    };

}

#endif // MODULES_H
