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

#ifndef FILEWRITERS_H
#define FILEWRITERS_H

#include "common.h"

#include <fstream>
#include <cmath>

namespace sb {
	template <typename Stream, typename Arrai>
	void byteArrayWrite (Stream& river, Arrai str) {
		if (str.length() == 0)
			return;
		for(size_t i = 0; i < str.length() - 1; ++i)
			river.put(str[i] | Bit1);
		river.put(str.back());
	}
}

#endif // FILEWRITERS_H
