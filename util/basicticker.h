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

#ifndef BASICTICKER_H
#define BASICTICKER_H

namespace sb {
	template <typename T, typename Internal = T>
	class BasicTicker {
	private:
		Internal pos;
		Internal speed;
	public:
		BasicTicker() {
			reset();
			speed = 0;
		}
		void setPos(T p) {
			pos = static_cast<Internal>(p);
		}
		void setSpeed(T s) {
			speed = static_cast<Internal>(s);
		}
		T tick() {
			pos += speed;
			return static_cast<T>(pos);
		}
		T getValue() {
			return static_cast<T>(pos);
		}
		void reset() {
			pos = 0;
		}
	};

	typedef sb::BasicTicker<float> FloatTicker;
	typedef sb::BasicTicker<int> IntTicker;
}

#endif // BASICTICKER_H
