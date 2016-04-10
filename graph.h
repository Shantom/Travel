#ifndef GRAPH_H
#define GRAPH_H
#include<vector>
#include <iostream>
#include <algorithm>
#include <QWidget>
#define A_BIG_INT 0x3f3f3f3f
using namespace std;

struct EdgeType {
    int cost = A_BIG_INT;
};
//邻接矩阵
class Graph {
public:
    Graph();
    void CreateGraph_MinCost();
    int Dijkstra(QString start_city, QString end_city, vector<QString> &out);
    int LeastCost(QString start_city, QString end_city, vector<QString> &mid_city, int isOrdered, vector<QString> &rout);

private:
    vector<QString> vertex;//城市名表
    EdgeType arc_MinCost[100][100];//邻接矩阵，可看作边表
    int numVertex;//图中当前的顶点数
    int numEdge; //图中当前的边数
};

#endif // GRAPH_H
