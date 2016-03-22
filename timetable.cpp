#include "timetable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
TimeTable::TimeTable()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); // 使用mysql数据库驱动
    db.setHostName("localhost");
    db.setDatabaseName("travel"); // 数据库名称
    db.setUserName("root"); //  用户名
    db.setPassword("sushe322"); // 密码
    bool ok = db.open(); // 尝试连接数据库

}
