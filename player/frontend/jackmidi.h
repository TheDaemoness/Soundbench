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


#ifndef JACKMIDI_H
#define JACKMIDI_H

#include "base.h"
#include "util/jackbase.h"

#ifndef NO_JACK

namespace sb {
	class JackMidiFrontend : public MidiFrontend, public JackBase {
	public:
		static bool instantiable();

		explicit JackMidiFrontend(Synth* s, size_t porte = 0);
		~JackMidiFrontend();
		void record(bool b);
		size_t getCurrentPort();
		void stop();
		void start();
		bool isRecording();
		bool supportsVirtualPorts();
		void setVirtualPort(bool vport);
		std::vector<std::string> getPorts();

	private:
	};
}
#else
namespace sb {
	class JackMidiFrontend : public MidiFrontend {
	public:
		static bool instantiable();

		explicit JackMidiFrontend(Synth* syn, size_t port = 0) : MidiFrontend(syn,port) {}
		~JackMidiFrontend() {}
		void stop() {}
		void start() {}
		void record(bool) {}
		size_t getCurrentPort() {return 0;}
		virtual std::vector<std::string> getPorts() {return std::vector<std::string>();}
	};
}
#endif

#endif // JACKMIDI_H

