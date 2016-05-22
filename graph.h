#ifndef GRAPH_H
#define GRAPH_H
#include<vector>
#include <iostream>
#include <algorithm>
#include <QWidget>
#include <QTime>
#define A_BIG_INT 0x3f3f3f3f
using namespace std;

struct EdgeType {
    int cost = A_BIG_INT;
    QTime start_time,end_time;
};
//邻接矩阵
class Graph {
public:
    Graph();
    void CreateGraph();

    int Dijkstra1(QString start_city, QString end_city, vector<QString> &out);
    int Dijkstra2(QString start_city, QString end_city, vector<QString> &out, double start_time);
    int LeastCost(QString start_city, QString end_city, vector<QString> &mid_city, int isOrdered, vector<QString> &rout);
    int LeastTime(double start_time, QString start_city, QString end_city, vector<QString> &mid_city, int isOrdered, vector<QString> &rout);
private:
    vector<QString> vertex;//城市名表
    vector<EdgeType> arc[100][100];//邻接矩阵，可看作边表
    int numVertex;//图中当前的顶点数
    int numEdge; //图中当前的边数
};

#endif // GRAPH_H
