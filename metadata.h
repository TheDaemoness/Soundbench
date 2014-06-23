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

	Copyright 2014  Amaya S.
*/

#ifndef METADATA_H
#define METADATA_H

#define SBVERSION "Development Version"
#define SBVERSION_CODENAME "Phoenix"
#define SBVERSION_MAJOR 0
#define SBVERSION_MINOR 5
#define SBVERSION_PATCH 0

#if defined(__linux__)
	#define SB_ENV_LINUX
	#define SB_ENV_POSIX
#elif defined (__APPLE__) & defined (__MACH__)
	#define SB_ENV_MACOS
	#define SB_ENV_POSIX
#elif defined(__CYGWIN__)
	#define SB_ENV_WNDOS
	#define SB_ENV_POSIX
#elif defined(_WIN32)
	#define SB_ENV_WNDOS
#elif defined(_POSIX_VERSION)
	#define SB_ENV_POSIX
#endif

#if defined(NO_RTMIDI)
#define NO_MIDIFRONTEND
#endif

#if defined(NO_RTAUDIO) & defined(NO_PORTAUDIO) & defined(NO_JACK)
#define NO_AUDIOBACKEND
#endif

#endif // METADATA_H
