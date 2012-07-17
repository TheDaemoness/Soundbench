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
#include <cstdio>

void SoundbenchMain::refreshPresets() {
    remove((datadir+"/presetrecord").c_str()); //Get rid of a certain preset record.
    loadPresetList();
}

void SoundbenchMain::resetBlueprint() {
    stopAndReset();
    arch->planAllDefaults(blu);
    arch->buildSynth(syn,blu);

    ui->presetLabel->setText("Unnamed Preset");

    ui->gener1TypeBox->setCurrentIndex(0);
    ui->gener2TypeBox->setCurrentIndex(0);
    ui->gener3TypeBox->setCurrentIndex(0);
    ui->gener4TypeBox->setCurrentIndex(0);

    ui->gen1SettButton->setDisabled(true);
    ui->gen2SettButton->setDisabled(true);
    ui->gen3SettButton->setDisabled(true);
    ui->gen4SettButton->setDisabled(true);

    em->start();
}

void SoundbenchMain::loadPresetList() {
    QFile river((datadir+"/presetrecord").c_str());
    river.open(QIODevice::ReadWrite | QIODevice::Text);

    //Get the records from the record file in the application data directory.
    while (!river.atEnd()) {
        sb::PresetMeta m;
        QString entry = river.readLine();
        entry.chop(1); //Remove the trailing newline.

        if (entry.isEmpty())
            break;

        auto e = entry.split(QRegExp("\t")).toVector();
        switch(e.size()) {
        default:
            m.tags = e[4].toStdString();
        case 4:
            m.desc = e[3].toStdString();
        case 3:
            m.arti = e[2].toStdString();
        case 2:
            m.name = e[1].toStdString();
        case 1:
            m.path = e[0].toStdString();
            break;
        }

        presetlist.push_back(m);
    }
    river.close();

    //Confirm that the record is telling the truth.
    QDir presetdir((datadir+"/presets").c_str());
    auto filelist = presetdir.entryList(QStringList("*.sbp")).toVector();

    std::cerr << '\n';
    size_t missing(0), extra(0);

    //Check for missing files.
    for (size_t i = 0; i < presetlist.size();) {
        if(std::find(filelist.begin(),filelist.end(),QString(presetlist[i].path.c_str())) == filelist.end()) {
            ++missing;
            presetlist.erase(presetlist.begin() + i);
        }
        else
            ++i;
    }
    if (missing == 1)
        std::cerr << "1 file was found missing and was removed.\n";
    else if (missing != 0)
        std::cerr << missing << " files were found missing and were removed.\n";

    //Check for extra files and add them.
    std::vector<std::string> pathlist;
    for(auto n : presetlist)
        pathlist.push_back(n.path);

    for (size_t i = 0; i < static_cast<size_t>(filelist.size()); ++i) {
        if(std::find(pathlist.begin(),pathlist.end(),filelist[i].toStdString()) == pathlist.end()) {
            sb::PresetMeta n = arch->getMetadata(filelist[i].toStdString(),datadir+"/presets");
            if (!n.path.empty()) {
                ++extra;
                presetlist.push_back(n);
            }
        }
    }

    if (extra == 1)
        std::cerr << "1 extra preset was found and was added.\n";
    else if (extra != 0)
        std::cerr << extra << " extra presets were found and were added.\n";

    if (presetlist.size() == 0)
        std::cerr << "The presets directory has no presets!\n";
    if (presetlist.size() == 1)
        std::cerr << "1 preset was loaded.\n";
    else
        std::cerr << presetlist.size() << " presets were loaded.\n";
}


void SoundbenchMain::displayPresets() {
    ui->presetList->clear();
    for (sb::PresetMeta pres : presetlist)
        ui->presetList->addItem((pres.name + " by " + pres.arti + " - " + pres.desc).c_str());
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
