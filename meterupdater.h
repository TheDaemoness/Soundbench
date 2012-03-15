#ifndef METERUPDATER_H
#define METERUPDATER_H

#include <QProgressBar>
#include <queue>

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
    inline bool artificial_limit() {
        return !nolimit;
    }

public slots:
    void update();

private:
    QProgressBar* affectedbar;

#ifndef _WIN32
    rusage ruse;
    rlimit64 rimit;
    timespec dust;
    bool usehlimit;
    bool nolimit;
#else
#endif

    double time;
    uint64_t totaltime;

    std::queue<double> prevtime;
    std::queue<uint64_t> prevtotaltime;

    double avgtime, avgtotaltime;
};

#endif // METERUPDATER_H
