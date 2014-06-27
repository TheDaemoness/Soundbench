#ifndef SBUIMAIN_H
#define SBUIMAIN_H

#include <QMainWindow>
#include "soundbench.h"

namespace Ui {
	class SoundbenchUI;
}

class SoundbenchUI : public QMainWindow
{
	Q_OBJECT

public:
	explicit SoundbenchUI(sb::Soundbench* sbh, bool del);
	~SoundbenchUI();

private:
	Ui::SoundbenchUI *ui;
	sb::Soundbench* bench;
	bool shouldDel;
};

#endif // SBUIMAIN_H
