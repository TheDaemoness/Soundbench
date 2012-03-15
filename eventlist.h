#ifndef EVENTLIST_H
#define EVENTLIST_H

#include <QDialog>
#include <QStackedWidget>

namespace Ui {
    class EventList;
}

class EventList : public QDialog
{
    Q_OBJECT
    
public:
    explicit EventList(QStackedWidget* evbox, QWidget *parent = 0);
    ~EventList();
    
private:
    Ui::EventList *ui;
    QStackedWidget* affectedwidg;
};

#endif // EVENTLIST_H
