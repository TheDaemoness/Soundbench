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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#include "soundbenchmain.h"

class SoundbenchApp : public QApplication
{
public:
    bool notify(QObject* obj, QEvent* evnt) {
        try {
            return QApplication::notify(obj, evnt);
        }
        catch (sbException& e) {
            exceptionSoundbench(e);
            return true;
        }
        catch (std::exception& e) {
            exceptionStandard(e);
            return true;
        }
        catch (...) {
            exceptionUnknown();
            return true;
        }
    }
    enum SoundbenchFlags {
        Default = 0,
        NoRTAudio = 1,
        NoMIDI = 2
    };

    explicit SoundbenchApp(int argc, char** argv);
    ~SoundbenchApp();
    inline int run() {
        if (sb != nullptr)
            sb->show();
        return this->exec();
    }

    void newSoundbench(int flags = 0);
    void endSoundbench() {
        delete sb;
    }
private:
    void exceptionSoundbench(sbException& e);
    void exceptionStandard(std::exception& e);
    void exceptionUnknown();

    SoundbenchMain* sb;
};

#endif // APPLICATION_H
