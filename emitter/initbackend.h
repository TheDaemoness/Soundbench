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

#ifndef INITBACKEND_H
#define INITBACKEND_H

#include "emitter.h"

namespace sb {
	template <sb::EmitterType TypeEnum, class Backside>
	bool initBackend(Emitter* em) {
		em->em_type = TypeEnum;
		if (em->supported_apis[TypeEnum]) {
			em->backend = new Backside(em->syn,global_srate,em->supported_rates,OUTPUT_CHANS);
			if (em->backend->isReady())
				return true;
			else {
				delete em->backend;
				em->backend = nullptr;
				em->em_type = NO_EMITTER;
				return false;
			}
		}
		else {
			em->em_type = NO_EMITTER;
			return false;
		}
	}
}

#endif // INITBACKEND_H
