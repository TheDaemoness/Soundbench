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

static bool sortPresets(const sb::PresetMeta& a, const sb::PresetMeta& b) {
    if (a.name < b.name)
        return true;
    return false;
}

void SoundbenchMain::loadPresetList() {
    presetlist.clear();

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

    std::sort(presetlist.begin(),presetlist.end(),sortPresets);

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
    int oldsize = ui->presetList->count();
    int hilight = ((oldsize != 0)?ui->presetList->currentRow():0);
    \
    ui->presetList->clear();
    if (!ui->presetLine->text().isEmpty()) {
        const QString& str = ui->presetLine->text();

        QStringList elems = str.toLower().split(' ',QString::SkipEmptyParts);
        for (sb::PresetMeta pres : presetlist) {
            auto tags = QString((pres.name + ' ' + pres.arti + ' ' + pres.tags).c_str()).toLower().split(' ',QString::SkipEmptyParts);

            bool allmatch = true;
            for (const auto& tag : elems) {
                if(!tags.contains(tag)) {
                    allmatch = false;
                    break;
                }
            }
            if (allmatch)
                ui->presetList->addItem((pres.name + " by " + pres.arti + " - " + pres.desc).c_str());
        }

    }
    else {
        for (sb::PresetMeta pres : presetlist)
            ui->presetList->addItem((pres.name + " by " + pres.arti + " - " + pres.desc).c_str());
    }
    if (oldsize != 0 && oldsize == ui->presetList->count())
        ui->presetList->setCurrentRow(hilight);

}

void SoundbenchMain::resetSelectUI() {
    ui->saveButton->setDisabled(true);
    ui->deleteButton->setDisabled(true);

    ui->presetList->setCurrentRow(ui->presetList->currentRow(),QItemSelectionModel::Deselect);

    ui->presetLabel->setDisabled(true);
    ui->presetLabel->setText("Unnamed Preset");
}

void SoundbenchMain::editMetadata(bool ed) {
    if (ed) {
        disconnect(ui->presetLine,SIGNAL(textEdited(QString)),this,SLOT(displayPresets()));
        ui->presetLine->clear();
        displayPresets();
        ui->metadataBox->setEnabled(false);
        switch(ui->metadataBox->currentIndex()) {
        case 0:
            ui->filterLabel->setText("Name:");
            ui->presetLine->setText(presetdata.name.c_str());
            break;
        case 1:
            ui->filterLabel->setText("Artist:");
            ui->presetLine->setText(presetdata.arti.c_str());
            break;
        case 2:
            ui->filterLabel->setText("Description:");
            ui->presetLine->setText(presetdata.desc.c_str());
            break;
        case 3:
            ui->filterLabel->setText("Tags:");
            ui->presetLine->setText(presetdata.tags.c_str());
            break;
        }
    }
    else {
        switch(ui->metadataBox->currentIndex()) {
        case 0:
            ui->presetLabel->setEnabled(true);
            ui->presetLabel->setText(ui->presetLine->text());
            presetdata.name = ui->presetLine->text().toStdString();
            break;
        case 1:
            presetdata.arti = ui->presetLine->text().toStdString();
            break;
        case 2:
            presetdata.desc = ui->presetLine->text().toStdString();
            break;
        case 3:
            presetdata.tags = ui->presetLine->text().toStdString();
            break;
        }

        ui->presetLine->clear();
        displayPresets();
        connect(ui->presetLine,SIGNAL(textEdited(QString)),SLOT(displayPresets()));
        ui->filterLabel->setText("Filter Presets:");
        displayPresets();
        ui->metadataBox->setEnabled(true);
    }

}
