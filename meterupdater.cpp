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
    getrlimit64(RLIMIT_CPU,&rimit);
    if (rimit.rlim_cur == RLIM_INFINITY) {
        std::clog << "No soft limit on CPU usage. Checking for hard limit...\n";
        if (rimit.rlim_max == RLIM_INFINITY) {
            std::clog << "No hard limit on CPU usage. Using total time.\n";
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
        prevtotaltime.push_back(0);
}

#ifndef _WIN32
void MeterUpdater::update() {
    getrusage(RUSAGE_SELF,&ruse);


    time = ruse.ru_stime.tv_usec + ruse.ru_stime.tv_sec*1000000;
    if (!nolimit) {
        totaltime = usehlimit?rimit.rlim_cur*1000000:rimit.rlim_max*1000000;
    }
    else {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&dust);
        totaltime = dust.tv_nsec/1000 + dust.tv_sec*1000000;
        prevtotaltime.push_back(totaltime);
    }
    prevtime.push_back(time);

    avgtime = time - prevtime.front();
    avgtotaltime = totaltime - prevtotaltime.front();
    affectedbar->setValue(avgtime/avgtotaltime*1000.0);

    if (prevtime.size() >= 500) {
        prevtime.pop_front();
        prevtotaltime.pop_front();
    }
}
#endif
