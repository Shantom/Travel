#ifndef PASSENGER_H
#define PASSENGER_H
#include <QWidget>
#include <QTime>
#include <QPair>
using namespace std;

struct Status
{
    char transport;
    QString curCity;
    QString nextMove;
};

class Passenger
{
public:
    Passenger();
    enum POLICY{minTime,minCost,timeLimitCost};
    void setStart(QString s);
    QString getStart();
    void setEnd(QString e);
    QString getEnd();
    void setPolicy(int p);
    int getPolicy();
    void setLimitTime(double L);
    QTime getLimitTime();
    void setWayCities(QList<QPair<QString, double>> W);
    QList<QPair<QString, double>> getWayCities();
    void setSequence(bool checked);
    bool isSequence();


private:
    QString start;
    QString end;
    int policy;
    QTime limitTime;
    QList<QPair<QString, double>> wayCities;
    bool sequence;
    Status curStatus;

};

#endif // PASSENGER_H
