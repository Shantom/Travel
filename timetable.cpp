#include "timetable.h"
#include <algorithm>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMap>

QMap<QString,QString> TimeTable::full2Short({{"北京","BJ"},{"天津","TJ"},{"成都","CD"},
                                             {"哈尔滨","HA"},{"大连","DL"},{"武汉","WH"},
                                             {"银川","YC"},{"呼和浩特","HH"},{"乌鲁木齐","WL"},
                                             {"西安","XA"},{"台北","TB"},{"六安","LA"},{"济南","JN"}});

TimeTable::TimeTable()
{

    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL"); // 使用mysql数据库驱动
        db.setHostName("localhost");
//        db.setHostName("121.42.209.206");
        db.setDatabaseName("travel"); // 数据库名称
        db.setUserName("root"); //  用户名
        db.setPassword("sushe322"); // 密码
    }
    bool ok = db.open(); // 尝试连接数据库
    if(!ok)
        exit(0);

}
TimeTable::~TimeTable()
{
    //db.close();
}

vector<Info> TimeTable::getInfos(QString start,QString goal)
{
    QSqlQuery query;
    query.exec(QString("SELECT price, departtime, arrivetime FROM travel WHERE departcity = '%1' "
                       "AND arrivecity = '%2' order by price").arg(full2Short[start],full2Short[goal]));
    vector<Info> rsl;
    while(query.next())
    {
        Info tmp;
        tmp.price=query.value(0).toInt();
        tmp.departtime=query.value(1).toTime();
        tmp.arrivetime=query.value(2).toTime();
        tmp.departcity=start;
        tmp.arrivecity=goal;
        rsl.push_back(tmp);
    }
    return rsl;
}

Info TimeTable::getInfo_MinCost(QString start, QString goal)
{
    QSqlQuery query;
    query.exec(QString("SELECT trainnumber, departtime, arrivetime, price, id  FROM travel "
                       "WHERE departcity = '%1' AND arrivecity = '%2' order by price")
               .arg(full2Short[start],full2Short[goal]));
    query.next();
    Info tmp;
    tmp.trainnumber=query.value(0).toString();
    tmp.departcity=start;
    tmp.arrivecity=goal;
    tmp.departtime=query.value(1).toTime();
    tmp.arrivetime=query.value(2).toTime();
    tmp.price=query.value(3).toInt();
    tmp.id=query.value(4).toInt();
    return tmp;

}

