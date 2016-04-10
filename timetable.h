#ifndef TIMETABLE_H
#define TIMETABLE_H
#include "graph.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>
#include <QTime>
#include <QDebug>

struct Info
{
    QString trainnumber;
    QString departcity;
    QString arrivecity;
    QTime departtime;
    QTime arrivetime;
    int price;
    int id;
};

class TimeTable
{
public:
    TimeTable();
    ~TimeTable();
    static int getMinPrice(QString start, QString goal);
    static Info getInfo_MinCost(QString start, QString goal);
private:
    QSqlDatabase db;
    static QMap<QString,QString> full2Short;
};

#endif // TIMETABLE_H
