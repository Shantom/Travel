#include "graph.h"
#include "timetable.h"
#include<vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <QString>
using namespace std;


Graph::Graph()
{
    TimeTable T;
}


//mid_city数组 要全部初始化成0，再记录城市；
//shunxu=true即有顺序，false即无顺序
//vector<QString>rout;//路线数组

/*重要：该算法只对强连通图有效*/
int Graph::LeastCost(QString start_city, QString end_city, vector<QString> &mid_city, int isOrdered, vector<QString> &rout)//返回最少费用
{
    int minCost = 0;
    int min_cost = A_BIG_INT;
    vector<QString>all_city;//临时数组
    vector<QString>r;
    vector<QString>part;//分段路线

    if (!mid_city.empty())  //如果有中间城市
    {
        if (!isOrdered)  // 无顺序
        {
            sort(mid_city.begin(), mid_city.end());

            do//全排列
            {
                //合成数组，将起点终点加在头尾
                all_city.clear();
                all_city.push_back(start_city);
                for (auto a : mid_city)
                    all_city.push_back(a);
                all_city.push_back(end_city);

                //计算费用
                minCost=0;
                for (auto i = all_city.begin(); i + 1 < all_city.end(); ++i)
                {
                    minCost += Dijkstra(*i, *(i + 1), part);
                    r.push_back(*i);
                    r.insert(r.end(), part.begin(), part.end());//将经过的路线追加到r上
                    part.clear();
                }
                r.push_back(*(all_city.end() - 1));//追加终点

                if (min_cost > minCost)
                {
                    min_cost = minCost;
                    rout.assign(r.begin(), r.end());//复制到rout里
                }
                r.clear();
            } while (next_permutation(mid_city.begin(), mid_city.end()));
            return min_cost;
        }
        else  //有顺序
        {
            //合成数组，将起点终点加在头尾
            all_city.push_back(start_city);
            for (auto a : mid_city)
                all_city.push_back(a);
            all_city.push_back(end_city);

            //计算费用
            for (auto i = all_city.begin(); i + 1 < all_city.end(); ++i)
            {
                minCost += Dijkstra(*i, *(i + 1), part);
                r.push_back(*i);
                r.insert(r.end(), part.begin(), part.end());//将经过的路线追加到r上
                part.clear();
            }
            r.push_back(*(all_city.end() - 1));//追加终点

            rout.assign(r.begin(), r.end());//复制到rout里
            return minCost;
        }
    }

    else//没有中间城市
    {
        rout.push_back(start_city);
        int rsl = Dijkstra(start_city, end_city, rout);
        rout.push_back(end_city);
        return rsl;
    }
}

int Graph::Dijkstra(QString start_city, QString end_city, vector<QString> &out)
{

    int P[100];  // P是各点的前驱节点的序号
    int D[100];   //D是起点到各点的路径长度

    int v_start = 0, v_end = 0;  //v_start对应start_city的邻接矩阵的序号
    int v; //顶点v(某个中间顶点)
    int w, k, min;
    int final[100];

    //寻找v_start //寻找v_end
    while (vertex[v_start] != start_city)
        v_start++;
    while (vertex[v_end] != end_city)
        v_end++;

    //初始化
    for (v = 0; v < numVertex; v++)
    {
        final[v] = 0;
        D[v] = arc_MinCost[v_start][v].cost;
        P[v] = v_start;
    }
    final[v_start] = 1;
    D[v_start] = 0;

    //求v_start到v的最短路径
    for (v = 0; v < numVertex; v++)
    {
        //v不能等于v_start
        if (v == v_start&&v < numVertex - 1)
            v++;
        if (v >= numVertex)
            break;

        min = A_BIG_INT;
        for (w = 0; w < numVertex; w++) //离v_start最近的点k(k不是已经确定的final点)
        {
            //不是起点v_start
            if (w == v_start)
                w++;
            if (w >= numVertex)
                break;

            if (!final[w] && D[w] < min)
            {
                k = w;
                min = D[w];
            }
        }
        final[k] = 1;

        //以k为前驱，修正当前最短路径及距离
        for (w = 0; w < numVertex; w++)
        {
            if (!final[w] && (min + arc_MinCost[k][w].cost < D[w]))
            {
                D[w] = min + arc_MinCost[k][w].cost;
                P[w] = k;
            }
        }
    }

    //找出路径
    //out数组存放路径节点序号， temp数组存放倒叙
    int temp[100];
    int i, t = v_end;
    for (i = 0; P[t] != v_start; i++)
    {
        temp[i] = P[t];
        t = P[t];
    }
    for (; i > 0; i--)
    {
        out.push_back(vertex[temp[i - 1]]);
    }

    return D[v_end];//最少费用

}

void Graph::CreateGraph_MinCost()
{
    vertex.assign({"北京","天津","成都","哈尔滨","大连","武汉",
                   "银川","呼和浩特","乌鲁木齐",
                   "济南","西安","台北","六安"});
    numVertex=vertex.size();

    for (int i = 0; i < numVertex; i++)
        arc_MinCost[i][i].cost = 0;


    //建立有向网的邻接矩阵表示
    int i, j;


    for (i = 0; i < numVertex; i++)
        for(j=0;j<numVertex;j++)
        {
            if(i!=j)
                arc_MinCost[i][j].cost = TimeTable::getMinPrice(vertex.at(i),vertex.at(j));
            else
                arc_MinCost[i][j].cost = 0;
        }
}
