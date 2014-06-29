#ifndef SBUIMAIN_H
#define SBUIMAIN_H

#include <QMainWindow>
#include "soundbench.h"
#include "config/config.h"

namespace Ui {
	class SoundbenchUI;
}

class SoundbenchUI : public QMainWindow
{
	Q_OBJECT

public:
	explicit SoundbenchUI(sb::Soundbench* sbh, sb::ConfigManager* cfg);
	~SoundbenchUI();

private:
	Ui::SoundbenchUI *ui;
	sb::Soundbench* bench;
	sb::ConfigManager* cfg;
	bool shouldDel;
};

#endif // SBUIMAIN_H
