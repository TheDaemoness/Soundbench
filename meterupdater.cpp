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

    nolimit = false;
#ifndef _WIN32
    getrlimit64(RLIMIT_CPU,&rimit);
    if (rimit.rlim_cur == RLIM_INFINITY) {
        std::clog << "No soft limit on CPU usage. Checking for hard limit...\n";
        if (rimit.rlim_max == RLIM_INFINITY) {
            std::clog << "No hard limit on CPU usage. Using true CPU time as upper bound of CPU meter.\n";
            nolimit = true;
        }
        else {
            std::clog << "Using hard limit on CPU usage as upper bound of CPU meter.\n";
            usehlimit = true;
        }
    }
    else {
        std::clog << "Using soft limit on CPU usage as upper bound of CPU meter.\n";
        usehlimit = false;
    }
    if (!nolimit)
        prevtotaltime.push(0);
#else
#endif
}

#ifndef _WIN32
void MeterUpdater::update() {
    getrusage(RUSAGE_SELF,&ruse);
    time = ruse.ru_utime.tv_usec + ruse.ru_utime.tv_sec*1000000;

    getrusage(RUSAGE_THREAD,&ruse);
    time -= ruse.ru_utime.tv_usec + ruse.ru_utime.tv_sec*1000000;

    if (!nolimit) {
        totaltime = usehlimit?rimit.rlim_cur*1000000:rimit.rlim_max*1000000;
    }
    else {
        clock_gettime(CLOCK_REALTIME,&dust);
        totaltime = dust.tv_nsec/1000 + dust.tv_sec*1000000;
        prevtotaltime.push(totaltime);
    }
    prevtime.push(time);

    avgtime = time - prevtime.front();
    avgtotaltime = totaltime - prevtotaltime.front();
    affectedbar->setValue(avgtime/avgtotaltime*1000.0);

    if (prevtime.size() >= 400) { //Maybe add some way of tuning this value in the future?
        prevtime.pop();
        prevtotaltime.pop();
    }
}
#else
void MeterUpdater::update() {
    affectedbar->setValue(0);
}
#endif
