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

#include "application.h"
#include "errorpopup.h"

SoundbenchApp::SoundbenchApp(int argc, char** argv) : QApplication(argc,argv) {
    sb = nullptr;
}

void SoundbenchApp::newSoundbench(int flags) {
    try {
        if (flags == 0) {
            sb = new SoundbenchMain;
            sb->delayedConstructor();
        }
    }
    catch (sbException& e) {
        exceptionSoundbench(e);
        return;
    }
    catch (std::exception& e) {
        exceptionStandard(e);
        return;
    }
    catch (...) {
        exceptionUnknown();

    }
}

SoundbenchApp::~SoundbenchApp() {
    delete sb;
}

void SoundbenchApp::exceptionSoundbench(sbException& e) {
    ErrorPopup* err = new ErrorPopup;
    err->setErrorText(e.errdata.first);
    err->setInfoText(e.errdata.second);
    for(sb::errs::ProblemFix* f : e.fixlist)
        err->addFix(f);
    err->exec();
    if (err->wasFixed()) {
        delete err;
        return;
    }
    else {
        delete err;
        abort();
    }
}

void SoundbenchApp::exceptionStandard(std::exception& e) {
    ErrorPopup* err = new ErrorPopup;
    err->setErrorText("Fatal Error");
    err->setInfoText(std::string("Details: ")+e.what());
    err->exec();
    delete err;
    abort();
}

void SoundbenchApp::exceptionUnknown() {
    ErrorPopup* err = new ErrorPopup;
    err->setErrorText("Unknown Error");
    err->setInfoText("An unknown error caused Soundbench to have some unresolvable problems.\n\nThis is an internal error so if you see this without having changed the program yourself, please report it as a bug.");
    err->exec();
    delete err;
    abort();
}
