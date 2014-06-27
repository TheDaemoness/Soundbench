#include "sbuimain.h"
#include "ui_sbuimain.h"

SoundbenchUI::SoundbenchUI(sb::Soundbench* sbh, bool del) :
	QMainWindow(nullptr), bench(sbh),
	ui(new Ui::SoundbenchUI)
{
	this->shouldDel = del;
	ui->setupUi(this);
}

SoundbenchUI::~SoundbenchUI()
{
	delete bench;
	delete ui;
}
