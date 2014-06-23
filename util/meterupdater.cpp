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

#ifdef SB_ENV_MACOS
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

MeterUpdater::MeterUpdater(QProgressBar* bare, QObject *parent) :
	QObject(parent)
{
	affectedbar = bare;
	time = 0.0;
	totaltime = 0;
}

#if defined(SB_ENV_POSIX)
void MeterUpdater::update() {
	//Get the CPU time for this process.

#if defined(SB_ENV_MACOS)
	getrusage(RUSAGE_SELF,&ruse);
#else
	getrusage(RUSAGE_SELF,&ruse);
	time = ruse.ru_utime.tv_usec + ruse.ru_utime.tv_sec*1000000;
#endif

	//Ignore this thread.
#if defined(SB_ENV_MACOS)
	task_basic_info task_inf;
	mach_msg_type_number_t dracula = TASK_BASIC_INFO_COUNT;

	if (task_info(mach_task_self(),TASK_BASIC_INFO, (task_info_t)&task_inf, &dracula) == KERN_SUCCESS)
		time -= task_inf.user_time.microseconds + task_inf.user_time.seconds*1000000;
#elif defined(SB_ENV_LINUX)
	getrusage(RUSAGE_THREAD,&ruse);
	time -= ruse.ru_utime.tv_usec + ruse.ru_utime.tv_sec*1000000;
#endif

	//Get total system time.
#ifdef SB_ENV_MACOS // OS X does not have clock_gettime
	dust = orwl_gettime();
#else
	clock_gettime(CLOCK_REALTIME,&dust);
#endif
	totaltime = dust.tv_nsec/1000 + dust.tv_sec*1000000;

	//A bit of value averaging.
	prevtotaltime.push(totaltime);
	prevtime.push(time);
	avgtime = time - prevtime.front();
	avgtotaltime = totaltime - prevtotaltime.front();

	affectedbar->setValue(avgtime/avgtotaltime*1000.0);
	while (prevtime.size() >= 450) { //Only one queue is checked because both queues should maintain the same size.
		prevtime.pop();
		prevtotaltime.pop();
	}
}
#ifdef SB_ENV_MACOS
struct timespec MeterUpdater::orwl_gettime(void) {
	// be more careful in a multithreaded environement
	if (!orwl_timestart) {
		mach_timebase_info_data_t tb = { 0 };
		mach_timebase_info(&tb);
		orwl_timebase = tb.numer;
		orwl_timebase /= tb.denom;
		orwl_timestart = mach_absolute_time();
	}
	struct timespec t;
	double diff = (mach_absolute_time() - orwl_timestart) * orwl_timebase;
	t.tv_sec = diff * ORWL_NANO;
	t.tv_nsec = diff - (t.tv_sec * ORWL_GIGA);
	return t;
}
#endif

#elif defined(SB_ENV_WNDOS)
static uint64_t filetime_to_ull(const FILETIME &ft) {
	uint64_t hi=ft.dwHighDateTime;
	return (uint64_t)(ft.dwLowDateTime)|(hi<<32);
}

void MeterUpdater::update() {
	// Get process time.
	GetProcessTimes(GetCurrentProcess(),&ft_unused,&ft_unused,&ft_kernel,&ft_user);
	time=filetime_to_ull(ft_kernel)+filetime_to_ull(ft_user);

	// Ignore this thread.
	GetThreadTimes(GetCurrentThread(),&ft_unused,&ft_unused,&ft_kernel,&ft_user);
	time-=filetime_to_ull(ft_kernel)+filetime_to_ull(ft_user);

	// Get total system time.
	GetSystemTimes(&ft_unused,&ft_kernel,&ft_user);
	totaltime=filetime_to_ull(ft_kernel)+filetime_to_ull(ft_user);

	//A bit of value averaging.
	prevtotaltime.push(totaltime);
	prevtime.push(time);
	avgtime = time - prevtime.front();
	avgtotaltime = totaltime - prevtotaltime.front();

	affectedbar->setValue(avgtime/avgtotaltime*1000.0);
	while (prevtime.size() >= 450) { //Only one queue is checked because both queues should maintain the same size.
		prevtime.pop();
		prevtotaltime.pop();
	}

}
#else
void MeterUpdater::update() {
	affectedbar->setValue(0);
}
#endif
