#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(double times, QObject *parent = 0);
    void StartTimer();
signals:
    void timerTick(int timer);
    void timerStopped();
    void timerStart();
public slots:
    void CountTimer();
private:
    QTimer m_timer;
    int m_tickedTime=0;
    int m_toTick;
};

#endif // TIMER_H
