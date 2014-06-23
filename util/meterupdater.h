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

#ifndef METERUPDATER_H
#define METERUPDATER_H

#include <QProgressBar>
#include <queue>

#include "sbutilities.h" //For the environment macros.

#if defined(SB_ENV_POSIX)
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(SB_ENV_WNDOS)
#define _WIN32_WINNT 0x0501
#include <windows.h>
#else
#ifdef __GNUC__
#warning The CPU meter will not work properly as this program is being built without POSIX or Windows APIs.
#endif
#endif

class MeterUpdater : public QObject
{
	Q_OBJECT
public:
	explicit MeterUpdater(QProgressBar* bare, QObject *parent = 0);
	void reset() {
		while(!prevtime.empty())
			prevtime.pop();
		while(!prevtotaltime.empty())
			prevtotaltime.pop();
		time = 0.0;
		totaltime = 0;
	}

public slots:
	void update();

private:
	QProgressBar* affectedbar;

#ifdef SB_ENV_MACOS
	static constexpr double ORWL_NANO = +1.0E-9;
	static constexpr uint64_t ORWL_GIGA = 1000000000;

	double orwl_timebase = 0.0;
	uint64_t orwl_timestart = 0;

	timespec orwl_gettime(void);
#endif

#if defined(SB_ENV_POSIX)
	rusage ruse;
	timespec dust;
#elif defined(SB_ENV_WNDOS)
	FILETIME ft_unused, ft_kernel, ft_user;
#endif
	double time;
	uint64_t totaltime;

	std::queue<double> prevtime;
	std::queue<uint64_t> prevtotaltime;

	double avgtime, avgtotaltime;
};

#endif // METERUPDATER_H
