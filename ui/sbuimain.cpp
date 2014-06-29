#include "sbuimain.h"
#include "ui_sbuimain.h"

SoundbenchUI::SoundbenchUI(sb::Soundbench* sbh, sb::ConfigManager* cfg) :
	QMainWindow(nullptr), bench(sbh), cfg(cfg),
	ui(new Ui::SoundbenchUI)
{
	ui->setupUi(this);
}

SoundbenchUI::~SoundbenchUI()
{
	delete ui;
}
