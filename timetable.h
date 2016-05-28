#ifndef TIMETABLE_H
#define TIMETABLE_H
#include "graph.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>
#include <QTime>
#include <QDebug>
#include "graph.h"

using namespace std;
struct EdgeType;
//{
//    QString trainnumber;
//    QString departcity;
//    QString arrivecity;
//    QTime start_time;
//    QTime end_time;
//    int price;
//    int id;
//};

class TimeTable
{
public:
    TimeTable();
    ~TimeTable();
    static vector<EdgeType> getInfos(QString start, QString goal);
    static EdgeType getInfo_MinCost(QString start, QString goal);
private:
    QSqlDatabase db;
    static QMap<QString,QString> full2Short;
};

#endif // TIMETABLE_H
