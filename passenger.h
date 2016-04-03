#ifndef PASSENGER_H
#define PASSENGER_H
#include <QWidget>
#include <QTime>
#include <QPair>
using namespace std;

struct Status
{
    char transport;//当前交通工具
    QString curCity;//当前城市
    QString nextMove;//下一步
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
    double getLimitTime();
    void setWayCities(QList<QPair<QString, double>> W);
    QList<QPair<QString, double>> getWayCities();
    void setSequence(bool checked);
    bool isSequence();

private:
    QString start;//起点
    QString end;//终点
    int policy;//策略
    double limitTime;//限时最低价格的限制时间
    QList<QPair<QString, double>> wayCities;//途经城市
    bool sequence;//是否有顺序
    Status curStatus;//暂时打算用作储存当前状态
};

#endif // PASSENGER_H
