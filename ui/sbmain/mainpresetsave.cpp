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

#include "soundbenchmain.h"

#include <fstream>

void SoundbenchMain::savePreset() {
    if (presetdata.path.empty())
        return;
    arch->savePreset(presetdata.path,
                     "",blu,
                     presetdata.name,
                     presetdata.arti,
                     presetdata.desc);
}

void SoundbenchMain::exportPreset() {
    ui->presetLine->releaseKeyboard();
    std::string newname = QFileDialog::getSaveFileName(NULL,"Choose a file to save a copy to...", (datadir+"/presets").c_str(), "Soundbench Presets (*.sbp)").toStdString();
    ui->presetLine->grabKeyboard();

    if(newname.empty())
        return;
    if(newname.find_last_of('.') != newname.size()-4)
        newname += ".sbp";
    presetdata.path = newname;

    arch->savePreset(newname,
                     "",blu,
                     presetdata.name,
                     presetdata.arti,
                     presetdata.desc);

    ui->saveButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}

void SoundbenchMain::deletePreset() {
    WarningPopup popu;
    popu.setWarningText("Confirm Deletion");
    popu.setInfoText("Once you delete a preset, the action cannot be trivially undone.\n\nIgnoring this warning will also reset the current preset.");
    popu.exec();

    if (!popu.fixed())
        return;

    QFile delfile;
    delfile.setFileName(presetdata.path.c_str());
    delfile.remove();
    loadPresetList(); //Reload the list
    resetBlueprint();

    ui->deleteButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    writePresetRecord();
    displayPresets();
    loadPresetList();
}

void SoundbenchMain::writePresetRecord() {
    std::ofstream of(datadir+"/presetrecord",std::ios_base::trunc);
    if (!of.is_open()) {
        std::cerr << "Something wicked happened while writing the preset record.\n";
        std::cerr << "Please check to ensure that Soundbench didn't lose permissions to write to its own directory.\n";
        return;
    }
    for (sb::PresetMeta pres : presetlist)
        of << pres.path << '\t' << pres.name << '\t'<< pres.arti << '\t' << pres.desc << '\t' << pres.tags << '\n';
}
