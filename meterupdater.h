#ifndef METERUPDATER_H
#define METERUPDATER_H

#include <QProgressBar>
#include <deque>

#ifndef _WIN32
    #include <sys/time.h>
    #include <sys/resource.h>
#else
#endif

class MeterUpdater : public QObject
{
    Q_OBJECT
public:
    explicit MeterUpdater(QProgressBar* bare, QObject *parent = 0);

public slots:
    void update();

private:
    QProgressBar* affectedbar;

#ifndef _WIN32
    rusage ruse;
    rlimit64 rimit;
    timespec dust;
#else
#endif

    double time;
    uint64_t totaltime;

    std::deque<double> prevtime;
    std::deque<uint64_t> prevtotaltime;

    bool usehlimit;
    bool nolimit;
    double avgtime, avgtotaltime;
};

#endif // METERUPDATER_H
