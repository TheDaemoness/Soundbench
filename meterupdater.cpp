#include "meterupdater.h"
#include "common.h"

MeterUpdater::MeterUpdater(QProgressBar* bare, time_t* teim, QObject *parent) :
    QObject(parent)
{
    the_time = teim;
    affectedbar = bare;
    zerocount = 0;
}

void MeterUpdater::update() {
    if (*the_time == 0)
        ++zerocount;
    else {
        affectedbar->setValue(*the_time);
        zerocount = 0;
    }
    if (zerocount > 1000) {
        affectedbar->setValue(0);
        zerocount = 0;
    }
}
