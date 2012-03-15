#include "eventlist.h"
#include "ui_eventlist.h"

EventList::EventList(QStackedWidget* evbox, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventList)
{
    ui->setupUi(this);
    affectedwidg = evbox;
}

EventList::~EventList()
{
    delete ui;
}
