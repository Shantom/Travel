#ifndef TIMELIMITCHEAPEST_H
#define TIMELIMITCHEAPEST_H
#include <QWidget>
#include <QVector>
#include "graph.h"
#include "widget.h"
struct way
{
    QVector<int> route;
    int len;
    int& operator[](unsigned x){return route[x];}
};//保存各顶点到终点的最短路径

extern way totalPath[13];//保存各顶点到终点的最短路径

struct a_star                //A*搜索时的优先级队列
{
    vector<int> pre;
    int v;
    int len;
    bool operator<(const a_star &b) const    //f(i)=g[i]+h(i)
    {
        return len + totalPath[v].len > b.len + totalPath[b.v].len;//small first
    }
};


class TimeLimitCheapest : public Graph
{
public:
    struct cmp    //f(i)=g[i]+h(i)
    {
        bool operator()(const a_star &a,const a_star &b){
        return a.len + totalPath[a.v].len > b.len + totalPath[b.v].len;//small first
        }
    };
    TimeLimitCheapest(Passenger Psg);
    bool AStar(QList<Status> &statuses,QString &detailRout);
private:
    void DijkstraFromT(int goal);
    double limit;
    bool isValid(a_star tmp, QList<Status> &statuses,QString &detailRout);
    int start,goal;
    QList<QPair<QString, double>> midCities;
};
void clearPath();

#endif // TIMELIMITCHEAPEST_H
