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

#ifndef GENTYPEDIALOG_H
#define GENTYPEDIALOG_H

#include <QDialog>
#include <QLabel>

#include "blueprint.h"

namespace Ui {
    class GenTypeDialog;
}

class GenTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GenTypeDialog(sb::Blueprint::gener_type*, sb::Blueprint::ModuleDataType*, QLabel*, QPushButton*, QWidget* parent = 0);
    ~GenTypeDialog();

private slots:
    void apply();

private:
    QLabel* affectedlabel;
    sb::Blueprint::gener_type* affectedgen;
    sb::Blueprint::ModuleDataType* affectedargs;
    QPushButton* affectedbutt; //Shh...
    Ui::GenTypeDialog *ui;
};

#endif // GENTYPEDIALOG_H
