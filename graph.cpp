#include "graph.h"
#include "timetable.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <QString>
#include <QMap>
using namespace std;


Graph::Graph()
{
    TimeTable T;
    vertex.assign({"北京","天津","成都","哈尔滨","大连","武汉",
                   "银川","呼和浩特","乌鲁木齐",
                   "济南","西安","台北","六安"});
    int i=0;
    for(auto a:vertex)
    {
        cityToInt[a]=i++;
    }

}

double time2Double(QTime t)
{
    return t.hour()+double(t.minute())/60.0;
}

//mid_city数组 要全部初始化成0，再记录城市；
//shunxu=true即有顺序，false即无顺序
//vector<QString>rout;//路线数组

/*重要：该算法只对强连通图有效*/
int Graph::LeastCost(QString start_city, QString end_city, vector<QString> &mid_city, bool isOrdered, vector<QString> &rout)//返回最少费用
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
                    minCost += Dijkstra1(*i, *(i + 1), part);
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
                minCost += Dijkstra1(*i, *(i + 1), part);
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
        int rsl = Dijkstra1(start_city, end_city, rout);
        rout.push_back(end_city);
        return rsl;
    }
}

int Graph::Dijkstra1(QString start_city, QString end_city, vector<QString> &out)
{

    int P[100];   //P是各点的前驱节点的序号
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
        D[v] = arc[v_start][v].empty()?A_BIG_INT:arc[v_start][v][0].price;
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
            if (!final[w] && (min + (arc[k][w].empty()?A_BIG_INT:arc[k][w][0].price) < D[w]))
            {
                D[w] = min + arc[k][w][0].price;
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
int Graph::Dijkstra2(QString start_city, QString end_city,
                     vector<QString> &out , double start_time,vector<EdgeType> &edgeList)
{

    int P[100];  // P是各点的前驱节点的序号
    double D[100];     //D是起点到各点的最早到达时间，

    int v_start = 0, v_end = 0;  //v_start对应start_city的邻接矩阵的序号
    int v; //顶点v(某个中间顶点)
    int i,w,k;
//    int all_time=0;//花费总时间
    int min;//从v_start到达 目前已知的final[w]=1的城市的时间点（某天某时刻）
//    int trival_time;//车上花费时间，结束时间减起始时间
    int final[100];

    //寻找v_start //寻找v_end
    while (vertex[v_start] != start_city)
        v_start++;
    while (vertex[v_end] != end_city)
        v_end++;

    QMap<QPair<int,int>,EdgeType> store;
    //初始化
    for (v = 0; v < numVertex; v++)
    {
        final[v] = 0;
        P[v] = v_start;
        D[v] = A_BIG_INT;
        //如果v和v_start之间有路线，就会进入循环
        //没有直达，D[v] =  INIFY
        for (auto i = arc[v_start][v].begin(); i < arc[v_start][v].end(); ++i)
        {
            if(time2Double(i->start_time) >= start_time)//当天有车
            {
                /*trival_time = (arc[v_start][v][i].end_time + 24 - arc[v_start][v][i].start_time)mod 24;
                D[v] =D[v] > trival_time? trival_time:D[v]; */

                //如果 end_time < start_time，则到达时间要+24
                if(i->start_time < i->end_time)
                {
                    if(D[v] > time2Double(i->end_time))
                    {
                        D[v]=time2Double(i->end_time);
                        store[QPair<int,int>{v_start,v}]=*i;
                    }
//                    D[v] =D[v] > time2Double(i->end_time) ? time2Double(i->end_time):D[v];
                }
                else
                {
                    if(D[v] > time2Double(i->end_time) + 24)
                    {
                        D[v]=time2Double(i->end_time) + 24;
                        store[QPair<int,int>{v_start,v}]=*i;
                    }

                }
//                    D[v] = D[v] > (time2Double(i->end_time) + 24) ? (time2Double(i->end_time) + 24):D[v];

            }
            else//过一夜看明天的车次有没有先到达的
            {
                if(i->start_time < i->end_time)
                {
                    if(D[v] > time2Double(i->end_time) + 24)
                    {
                        D[v]=time2Double(i->end_time) + 24;
                        store[QPair<int,int>{v_start,v}]=*i;
                    }

                }

//                    D[v] = D[v] > (time2Double(i->end_time) + 24) ? (time2Double(i->end_time) + 24):D[v];
                else
                {
                    if(D[v] > time2Double(i->end_time) + 48)
                    {
                        D[v]=time2Double(i->end_time) + 48;
                        store[QPair<int,int>{v_start,v}]=*i;
                    }

                }

//                    D[v] = D[v] > (time2Double(i->end_time) + 48) ? (time2Double(i->end_time) + 48):D[v];
            }
        }
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

            if (!final[w]  && D[w] < min)
            {
                k = w;
                min = D[w];//最早到达时间
            }
        }
        final[k] = 1;


        //以k为前驱，修正当前最短路径及距离
        for (w = 0; w < numVertex; w++)
        {
            //遍历 arc[k][w]的每重边
            for (auto i = arc[k][w].begin(); i < arc[k][w].end(); ++i)
            {
                //start_time >= min%24，w城市可以用k城市作为前驱 ，路线为第i重边

                /*如何算途径k到达w的抵达时间：
                    1.当天即可坐上从k到w的车：
                        a. 当天没有车 ，过一天（进入情况2）
                        b. 当天抵达w： 抵达时间为 ((min/24)*24 + i->end_time)
                        c. start_time > end_time： 抵达时间为 ((min/24)*24 + i->end_time)+24
                    2.过一夜看明天的车次有没有先到达的
                        a. 明天也没有
                        b. 明天有车，当天抵达，抵达时间为 ((min/24)*24 + i->end_time)+24
                        c. 明天有车，start_time > end_time，抵达时间为 ((min/24)*24 + i->end_time)+48
                */
                if (!final[w]  &&  time2Double(i->start_time) >= (min%24))//1.当天有车
                {
                    double p = (min/24)*24 + time2Double(i->end_time);
                    if((i->start_time <= i->end_time)&&(D[w]<p))//b
                        {
                            D[w] = p;
                            P[w] = k;
                            store[QPair<int,int>{k,w}]=(*i);
                        }
                    if((i->start_time >= i->end_time)&&(D[w] < p+24))//c
                        {
                            D[w] = p + 24;
                            P[w] = k;
                            store[QPair<int,int>{k,w}]=(*i);
                        }
                }

                else//2.过一天
                {
                    double p = (min/24)*24 + time2Double(i->end_time) + 24;
                    if((i->start_time <= i->end_time)&&(D[w] > p))//b
                    {
                        D[w] = p;
                        P[w] = k;
                        store[QPair<int,int>{k,w}]=(*i);
                    }
                    if((i->start_time >= i->end_time)&&(D[w] > p+24))//c
                    {
                        D[w] = p + 24;
                        P[w] = k;
                        store[QPair<int,int>{k,w}]=(*i);
                    }
                }
            }
        }
    }
    //找出路径
    //out数组存放路径节点序号， temp数组存放倒叙
    int temp[100];
    int t = v_end;
    i = v_end;
    edgeList.push_back(store[QPair<int,int>{P[t],t}]);
    for (i = 0; P[t] != v_start; i++)
    {
        temp[i] = P[t];
        t = P[t];
        edgeList.push_back(store[QPair<int,int>{P[t],t}]);
    }
    for (; i > 0; i--)
        out.push_back(vertex[temp[i - 1]]);
    std::reverse(edgeList.begin(),edgeList.end());

    return D[v_end];//最少时间

}

int Graph::LeastTime(double start_time,QString start_city, QString end_city,
                     vector<QString> &mid_city, bool isOrdered, vector<QString> &rout,vector<EdgeType> &edgeList)//返回最少费用
{
    double temptime = start_time;
    int minTime = 0;
    int min_time = A_BIG_INT;
    vector<QString>all_city;//临时数组
    vector<QString>r;
    vector<QString>part;//分段路线

    if (!mid_city.empty())  //如果有中间城市
    {
        vector<EdgeType> tmpEdgeList,partEdge;
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

                //计算时间
                minTime=0;
                for (auto i = all_city.begin(); i + 1 < all_city.end(); ++i)
                {
                    minTime = minTime - minTime%24 + Dijkstra2(*i, *(i + 1), part, temptime,partEdge);
                    r.push_back(*i);
                    r.insert(r.end(), part.begin(), part.end());//将经过的路线追加到r上
                    tmpEdgeList.insert(tmpEdgeList.end(),partEdge.begin(),partEdge.end());
                    partEdge.clear();
                    part.clear();
                    temptime= minTime % 24;
                }
                r.push_back(*(all_city.end() - 1));//追加终点

                if (min_time > minTime)
                {
                    min_time = minTime;
                    rout.assign(r.begin(), r.end());//复制到rout里
                    edgeList.assign(tmpEdgeList.begin(),tmpEdgeList.end());
                }
                r.clear();
                tmpEdgeList.clear();
                temptime = start_time;
            } while (next_permutation(mid_city.begin(), mid_city.end()));
            return min_time;
        }
        else  //有顺序
        {
            //合成数组，将起点终点加在头尾
            all_city.push_back(start_city);
            for (auto a : mid_city)
                all_city.push_back(a);
            all_city.push_back(end_city);

            //计算时间
            for (auto i = all_city.begin(); i + 1 < all_city.end(); ++i)
            {
                minTime = minTime - minTime%24 + Dijkstra2(*i, *(i + 1), part, start_time,partEdge);
                r.push_back(*i);
                r.insert(r.end(), part.begin(), part.end());//将经过的路线追加到r上
                edgeList.insert(edgeList.end(),partEdge.begin(),partEdge.end());
                partEdge.clear();
                part.clear();
                start_time = minTime % 24;
            }
            r.push_back(*(all_city.end() - 1));//追加终点

            rout.assign(r.begin(), r.end());//复制到rout里
            return minTime;
        }
    }

    else//没有中间城市
    {
        rout.push_back(start_city);
        int rsl = Dijkstra2(start_city, end_city, rout, start_time,edgeList);
        rout.push_back(end_city);
        return rsl;
    }
}

EdgeType Graph::getInfo_MinTime(QString start, QString goal, QTime curTime)
{
    vector<EdgeType> tmpEdges=arc[cityToInt[start]][cityToInt[goal]];

    EdgeType tmp;
    tmp.arrivecity=goal;
    tmp.departcity=start;

    for(EdgeType &a:tmpEdges)
    {
        a.start_time=a.start_time.addMSecs(-curTime.msecsSinceStartOfDay());
        a.end_time=a.end_time.addMSecs(-curTime.msecsSinceStartOfDay());
    }
    sort(tmpEdges.begin(),tmpEdges.end(),[](EdgeType a,EdgeType b)->bool{
        if((a.start_time>a.end_time&&b.start_time>a.end_time)||(a.start_time<a.end_time&&b.start_time<a.end_time))
                return a.end_time<b.end_time;
        else
                return a.start_time<b.start_time;});

    tmpEdges[0].start_time=tmpEdges[0].start_time.addMSecs(curTime.msecsSinceStartOfDay());
    tmpEdges[0].end_time=tmpEdges[0].end_time.addMSecs(curTime.msecsSinceStartOfDay());
    tmp.end_time=tmpEdges[0].end_time;
    tmp.start_time=tmpEdges[0].start_time;
    tmp.price=tmpEdges[0].price;
    tmp.trainnumber=tmpEdges[0].trainnumber;
    return tmp;

}
void Graph::CreateGraph()
{

    numVertex=vertex.size();

    //建立有向网的邻接矩阵表示
    int i, j;

    for (i = 0; i < numVertex; i++)
        for(j=0;j<numVertex;j++)
        {
            if(i!=j)
            {
                vector<EdgeType> tmpInfo=TimeTable::getInfos(vertex.at(i),vertex.at(j));
                for(auto a:tmpInfo)
                {
                    EdgeType tmpEdge;
                    tmpEdge.trainnumber=a.trainnumber;
                    tmpEdge.price=a.price;
                    tmpEdge.start_time=a.start_time;
                    tmpEdge.end_time=a.end_time;
                    tmpEdge.arrivecity=vertex.at(j);
                    tmpEdge.departcity=vertex.at(i);
                    arc[i][j].push_back(tmpEdge);
                }
            }
        }
}

EdgeType Graph::getInfo_MinCost(QString start, QString goal)
{
    int s=cityToInt[start];
    int g=cityToInt[goal];
    EdgeType tmp;
    tmp.trainnumber=arc[s][g][0].trainnumber;
    tmp.departcity=start;
    tmp.arrivecity=goal;
    tmp.start_time=arc[s][g][0].start_time;
    tmp.end_time=arc[s][g][0].end_time;
    tmp.price=arc[s][g][0].price;
    tmp.id=arc[s][g][0].id;
    return tmp;

}
