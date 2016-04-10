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

int TimeTable::getMinPrice(QString start,QString goal)
{
    QSqlQuery query;
    query.exec(QString("SELECT price FROM travel WHERE departcity = '%1' "
                       "AND arrivecity = '%2' order by price").arg(full2Short[start],full2Short[goal]));
    if(query.next())
        return query.value(0).toInt();
    else
        return A_BIG_INT;

//    QList<int> priceList;
//    while(query.next())
//    {
//        priceList.push_back(query.value(0).toInt());
//    }
//    sort(priceList.begin(),priceList.end());//如果order by影响效率则用这一段
}

TimeTable::Info getInfo_MinCost(QString start, QString goal)
{

}

