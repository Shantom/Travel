#include "timer.h"

Timer::Timer(double times, QObject *parent) : QObject(parent), m_toTick(times)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(CountTimer()));
    m_timer.setInterval(100);
    m_timer.setSingleShot(false);
}

void Timer::StartTimer()
{
    m_timer.start();
    emit timerStart();
}

void Timer::CountTimer()
{
    emit timerTick(m_tickedTime);
    ++m_tickedTime;
    if(m_tickedTime>m_toTick)
    {
        m_timer.stop();
        emit timerStopped();
    }
}
