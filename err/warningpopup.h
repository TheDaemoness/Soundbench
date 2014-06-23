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

#ifndef WARNINGPOPUP_H
#define WARNINGPOPUP_H

#include <QDialog>

#include "problemfix.h"

namespace Ui {
	class WarningPopup;
}

class WarningPopup : public QDialog
{
	Q_OBJECT

public:
	explicit WarningPopup(QWidget *parent = 0);
	~WarningPopup();
	void setWarningText(std::string);
	void setInfoText(std::string);
	void setFix(sb::errs::fixes::ProblemFix*);
	bool fixed() {
		return userchoice;
	}

private slots:
	void warnIgnore() {
		userchoice = true;
		this->close();
	}
	void warnAbort() {
		userchoice = false;
		this->close();
	}
	void warnFix() {
		thefix->runFix();
		userchoice = true;
		this->close();
	}

private:
	Ui::WarningPopup *ui;
	bool userchoice;
	bool fixexists;
	sb::errs::fixes::ProblemFix* thefix;
};

#endif // WARNINGPOPUP_H
