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

void Passenger::setLimitTime(QTime L)
{
    limitTime=L;
}

void Passenger::setWayCities(QList<QPair<QString, double>> W)
{
    wayCities=W;
}

void Passenger::setSequence(bool checked)
{
    sequence=checked;
}
