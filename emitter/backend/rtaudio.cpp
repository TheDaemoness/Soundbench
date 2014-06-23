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

#include "rtaudio.h"

namespace sb {
#ifndef NO_RTAUDIO
	bool RtAudioBackend::instantiable() {
		try {
			RtAudio audio;

			unsigned int devices = audio.getDeviceCount();
			if (devices == 0) {
				std::cerr << "RtAudio could not find any audio devices. Skipping this backend...\n";
				return false;
			}
			auto info = audio.getDeviceInfo(audio.getDefaultOutputDevice());
			if (info.probed) {
				if(info.outputChannels >= 2)
					return true;
				else if (info.outputChannels == 1)
					std::cerr << "The default device for RtAudio does not support stereo sound. Skipping this backend...\n";
				else
					std::cerr << "The default device for RtAudio cannot output any audio at this time. Skipping this backend...\n";
			}
			else
				std::cerr << "RtAudio could not find or probe the default audio device. Skipping this backend...\n";
		}
		catch (RtError& e) {
			std::cerr << "RtAudio threw an exception: " << e.getMessage() << ". Skipping this backend...\n";
		}
		return false;
	}

	RtAudioBackend::RtAudioBackend(Synth* syns, size_t& srate, std::map<size_t,bool>& supported_rates, size_t chans) {
		syn = syns;
		sampling_rate = srate;
		params.nChannels = chans;
		params.deviceId = rta.getDefaultOutputDevice();
		bufflen = sampling_rate/100;

		rta.showWarnings(true);

		for(auto& pear : supported_rates)
			pear.second = false;

		for(uint32_t srat : rta.getDeviceInfo(params.deviceId).sampleRates) {
			if (supported_rates.count(srat))
				supported_rates[srat] = true;
		}

		ready = true;
	}

	void RtAudioBackend::start() {
		try {
			if (!rta.isStreamOpen())
				rta.openStream(&params,nullptr,RTAUDIO_FLOAT32,sampling_rate,&bufflen,callback,reinterpret_cast<void*>(syn));
			if(!rta.isStreamRunning())
				rta.startStream();
			running = rta.isStreamRunning();
		}
		catch (RtError& e) {
			ready = false;
			std::cerr << "RtAudio had problems opening and starting a stream.\n";
			std::cerr << "Details: " << e.what() << "\n";
		}
	}
	void RtAudioBackend::stop() {
		if(rta.isStreamRunning())
			rta.abortStream();
		if (rta.isStreamOpen())
			rta.closeStream();
		running = rta.isStreamRunning();
	}

	int RtAudioBackend::callback( void *outputBuffer, void*, unsigned int nBufferFrames, double, RtAudioStreamStatus, void *userData) {
		reinterpret_cast<Synth*>(userData)->interleaved_block(reinterpret_cast<SbSample*>(outputBuffer),nBufferFrames);
		/*
	SbSample* framepointer = reinterpret_cast<SbSample*>(outputBuffer);
	for (size_t i = 0; i < nBufferFrames; framepointer += OutChannels, ++i)
		reinterpret_cast<Synth*>(userData)->tick(framepointer);
	*/
		return 0;
	}

	std::vector<std::string> RtAudioBackend::getDevices() {
		std::vector<std::string> naems;
		for(uint32_t i = 0; i < rta.getDeviceCount(); ++i) {
			if(rta.getDeviceInfo(i).probed)
				naems.push_back(rta.getDeviceInfo(i).name);
		}
		return naems;
	}

#else
	bool RtAudioBackend::instantiable() {
		return false;
	}
#endif
}
