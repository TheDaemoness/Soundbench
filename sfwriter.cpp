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

#include "sfwriter.h"

namespace sb {
    bool SoundFileWriter::open(std::string name, ExportableFiles ex) {
        SF_INFO params;
        params.samplerate = global_srate;
        params.channels = OutChannels;
        params.format = SF_FORMAT_PCM_32;
        if (ex == FileWAV)
            params.format |= SF_FORMAT_WAV;
        else if (ex == FileAIFF)
            params.format |= SF_FORMAT_AIFF;
        else if (ex == FileFLAC)
            params.format = SF_FORMAT_FLAC | SF_FORMAT_PCM_24; //WARNING: Do not remove the SF_FORMAT_PCM_24.
        else if (ex == FileRAW)
            params.format |= SF_FORMAT_RAW;
        else if (ex == FileMAT)
            params.format = SF_FORMAT_MAT5 | SF_FORMAT_FLOAT; //WARNING: Do not remove the SF_FORMAT_FLOAT.

        filehandel = sf_open(name.c_str(),SFM_WRITE,&params);
        if (filehandel == NULL) {
            filehandel = nullptr;
            return false;
        }
        return true;
    }
    void SoundFileWriter::put(SbSample* samp, size_t len) {
        for (size_t i = 0; i < len; ++i)
            samples.push_back(samp[i]);
        needsflush = true;
    }

    void SoundFileWriter::tick() {
        SbSample tmp[2];
        syn->tick(tmp);
        samples.push_back(tmp[0]);
        samples.push_back(tmp[1]);
        needsflush = true;
    }

    bool SoundFileWriter::flush() {
        if (filehandel == nullptr)
            return false;
        SbSample tmp[2];
        bool flipper = false;
        for(SbSample s : samples) {
            if (flipper) {
                tmp[1] = s;
                sf_writef_float(filehandel,tmp,1);
                flipper = false;
            }
            else {
                tmp[0] = s;
                flipper = true;
            }
        }
        samples.clear();
        needsflush = false;
        return true;
    }

    void SoundFileWriter::close() {
        if (needsflush)
            flush();
        if (filehandel != nullptr)
            sf_close(filehandel);
    }
}
