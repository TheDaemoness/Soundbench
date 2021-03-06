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

#ifndef NO_JACK
#ifndef JACKGLOBALS_H
#define JACKGLOBALS_H

#include <jack/jack.h>

namespace sb {
	class JackBase { //The static members in here are in main.cpp.
	protected:
		static jack_client_t* cli;
		static jack_status_t stat;
	};
}

#endif // JACKGLOBALS_H
#endif // #ifndef NO_JACK
