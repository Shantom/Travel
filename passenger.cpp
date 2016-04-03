#include "passenger.h"

Passenger::Passenger()
{

}
void Passenger::setStart(QString s)
{
    start=s;
}
QString Passenger::getStart()
{
    return start;
}

void Passenger::setEnd(QString e)
{
    end=e;
}
QString Passenger::getEnd()
{
    return end;
}

void Passenger::setPolicy(int p)
{
    policy=p;
}
int Passenger::getPolicy()
{
    return policy;
}

void Passenger::setLimitTime(double L)
{
    //limitTime=QTime((int)L,60*(L-(int)L));
    limitTime=L;
}

double Passenger::getLimitTime()
{
    return limitTime;
}

void Passenger::setWayCities(QList<QPair<QString, double>> W)
{
    wayCities=W;
}

QList<QPair<QString, double>>Passenger::getWayCities()
{
    return wayCities;
}

void Passenger::setSequence(bool checked)
{
    sequence=checked;
}

bool Passenger::isSequence()
{
    return sequence;
}
