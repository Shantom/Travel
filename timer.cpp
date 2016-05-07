#include "timer.h"

Timer::Timer(double times, QObject *parent) : QObject(parent), m_toTick(times)
{
    isPaused=false;
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(CountTimer()));
    m_timer.setInterval(100);
    m_timer.setSingleShot(false);
}

void Timer::StartTimer()
{
    m_timer.start();
    emit timerStart();
}

void Timer::StopTimer(bool isPause)
{
    m_timer.stop();
    emit timerStopped(isPause);
}

void Timer::CountTimer()
{
    emit timerTick(m_tickedTime);
    ++m_tickedTime;
    if(m_tickedTime>m_toTick)
    {
        StopTimer(false);
    }
}
