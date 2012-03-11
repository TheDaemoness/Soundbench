#ifndef METERUPDATER_H
#define METERUPDATER_H

#include <QProgressBar>

class MeterUpdater : public QObject
{
    Q_OBJECT
public:
    explicit MeterUpdater(QProgressBar* bare, time_t* teim, QObject *parent = 0);

public slots:
    void update();

private:
    QProgressBar* affectedbar;
    time_t* the_time;
    size_t zerocount;
    
};

#endif // METERUPDATER_H
