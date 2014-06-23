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

#ifndef FRONTEND_H
#define FRONTEND_H

#include "sbutilities.h"
#include "player/midichain.h"
#include "midifio/midievents.h"
#include "synth/synth.h"

namespace sb {

	class MidiFrontend  {
	public:
		MidiFrontend(Synth* s, size_t porte = 0) {
			running = false;
			portnum = porte;
			syn = s;
			foist = new midi::nodes::PlayerStartNode(syn);
		}
		virtual ~MidiFrontend() {
			delete foist;
		}

		virtual void record(bool) = 0;
		virtual void stop() = 0;
		virtual void start() = 0;
		virtual bool isRecording() {
			return false;
		}

		virtual bool supportsVirtualPorts() {
			return false;
		}
		virtual void setVirtualPort(bool vport) {
			usevport = vport;
		}

		virtual midi::nodes::PlayerStartNode* detachChain() {
			if (isRecording())
				return new midi::nodes::PlayerStartNode(syn);
			auto chein = foist;
			foist = new midi::nodes::PlayerStartNode(syn);
			return chein;
		}
		virtual size_t getCurrentPort() = 0;
		virtual bool isRunning() {
			return running;
		}
		virtual bool isReady() {
			return ready;
		}

		virtual void setPort(uint32_t ship) {
			portnum = ship;
		}
		virtual std::vector<std::string> getPorts() = 0;

	protected:
		midi::nodes::PlayerStartNode* foist;
		bool running, ready;
		uint32_t portnum;
		sb::Synth* syn;
		bool usevport;
	};

}

#endif // FRONTEND_H
