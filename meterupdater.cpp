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

#include "meterupdater.h"
#include "common.h"

#include <thread>

MeterUpdater::MeterUpdater(QProgressBar* bare, QObject *parent) :
    QObject(parent)
{
    affectedbar = bare;
    time = 0.0;
    totaltime = 0;
}

#ifdef SB_ENV_POSIX
void MeterUpdater::update() {
    //Get the CPU time for this process.
    getrusage(RUSAGE_SELF,&ruse);
    time = ruse.ru_utime.tv_usec + ruse.ru_utime.tv_sec*1000000;

    //Ignore this thread.
    getrusage(RUSAGE_THREAD,&ruse);
    time -= ruse.ru_utime.tv_usec + ruse.ru_utime.tv_sec*1000000;

    clock_gettime(CLOCK_REALTIME,&dust);
    totaltime = dust.tv_nsec/1000 + dust.tv_sec*1000000;

    //A bit of value averaging.
    prevtotaltime.push(totaltime);
    prevtime.push(time);
    avgtime = time - prevtime.front();
    avgtotaltime = totaltime - prevtotaltime.front();
    affectedbar->setValue(avgtime/avgtotaltime*1000.0);

    if (prevtime.size() >= 450) { //Only one queue is checked because both queues should maintain the same size.
        prevtime.pop();
        prevtotaltime.pop();
    }
}
#else
void MeterUpdater::update() {
    affectedbar->setValue(0);
}
#endif
