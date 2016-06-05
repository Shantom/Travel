#include "timelimitcheapest.h"
#include <queue>
way totalPath[13];//保存各顶点到终点的最短路径
bool flag;

TimeLimitCheapest::TimeLimitCheapest(Passenger Psg)
{
    limit=Psg.getLimitTime();
    start=cityToInt[Psg.getStart()];
    goal=cityToInt[Psg.getEnd()];
    midCities=Psg.getWayCities();
    CreateGraph();

    DijkstraFromT(goal);
}

bool TimeLimitCheapest::AStar(QList<Status> &statuses,QString &detailRout)
{
    flag=false;
re: a_star n1;
    n1.v = start;
    n1.len = 0;
    priority_queue<a_star> q;
    q.push(n1);
    vector<int> ans(13);
    int Count=0;
    while ((!q.empty())&&Count<=500000)//500000次以上就退出
    {
        a_star temp = q.top();

        if(isValid(temp,statuses,detailRout))
        {
            return true;
            clearPath();
        }

        q.pop();
        ans[temp.v]++;
        if(count(temp.pre.begin(),temp.pre.end(),0)>3)
        {
            Count--;
            continue;
        }
        for (int i=0;i<13;i++) {
            if(arc[temp.v][i].empty())
                continue;
            a_star n2;
            n2.pre.assign(temp.pre.begin(),temp.pre.end());
            n2.pre.push_back(temp.v);
            n2.v = i;
            n2.len = arc[temp.v][i][0].price + temp.len;
            q.push(n2);
        }
        Count++;
    }
    if(!flag)
    {
        flag=!flag;
        while(!q.empty())
            q.pop();
        goto re;
    }
    clearPath();
    return false;
}

void TimeLimitCheapest::DijkstraFromT(int goal)
{
    int P[100];   //P是各点的前驱节点的序号
    int D[100];   //D是起点到各点的路径长度

    int v_start = goal/*, v_end = 0*/;  //v_start对应goal
    int v; //顶点v(某个中间顶点)
    int w, k, min;
    int final[100];

    //初始化
    for (v = 0; v < numVertex; v++)
    {
        final[v] = 0;
        D[v] = arc[v][v_start].empty()?A_BIG_INT:arc[v][v_start][0].price;//强行反向
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
            if (!final[w] && (min + (arc[w][k].empty()?A_BIG_INT:arc[w][k][0].price) < D[w]))
            {
                D[w] = min + arc[w][k][0].price;
                P[w] = k;
            }
        }
    }

    for(int j=0;j<13;j++)
    {

        //找出路径
        //out数组存放路径节点序号， temp数组存放倒叙
        int i, t = j;
        for (i = 0; t != goal; i++)
        {
            totalPath[j].route.push_back(t);
            t = P[t];
        }
        totalPath[j].route.push_back(t);
        totalPath[j].len=D[j];
    }

}

bool TimeLimitCheapest::isValid(a_star tmp,QList<Status> &statuses,QString &detailRout)
{
    vector<int> all;
    all.assign(tmp.pre.begin(),tmp.pre.end());
    all.insert(all.end(),totalPath[tmp.v].route.begin(),totalPath[tmp.v].route.end());
    for(QPair<QString, double> city:midCities)
    {
        if(0==count(all.begin(),all.end(),cityToInt[city.first]))
            return false;
    }



re: QTime preArriTime(23,59,59);//上一个城市的到达时间
    int day=0;
    size_t j=0;//停留时间的标尺

    EdgeType iniInfo;
    if(!flag)
        iniInfo=getInfo_MinCost(vertex[all[0]],vertex[all[1]]/*,preArriTime*/);
    else
        iniInfo=getInfo_MinTime(vertex[all[0]],vertex[all[1]],preArriTime);


    double iniTime=iniInfo.start_time.hour()+double(iniInfo.start_time.minute())/60+24;

    for(auto i=all.begin();i<all.end()-1;++i)//route保存包括起点终点的所有城市名
    {


        QTime tmp=preArriTime;
        if(j<midCities.size()&&cityToInt[midCities.at(j).first]==*i)//如果当前城市为停留城市
        {
            preArriTime=preArriTime.addSecs(3600*midCities.at(j++).second);//加上停留时间
        }

        EdgeType section;
            section=getInfo_MinCost(vertex[*i],vertex[*(i+1)]/*,preArriTime*/);


        if(preArriTime>section.start_time||preArriTime<tmp)//判断是否需要第二天再走
        {
            day++;
            detailRout+=(Widget::tr("第%1天").arg(day)+'\n');
        }

        Status curStatus;
        //坐车时的状态
        curStatus.transport=section.trainnumber;
        curStatus.curCity=section.departcity+'-'+section.arrivecity;
        curStatus.startTime=section.start_time.hour()+double(section.start_time.minute())/60+24*day-iniTime;
        statuses.append(curStatus);

        preArriTime=section.end_time;

        detailRout+=(section.trainnumber+' ');
        detailRout+=(section.departcity+"到"+section.arrivecity+' ');
        detailRout+=(section.start_time.toString("HH:mm")+"到"+section.end_time.toString("HH:mm"));
        detailRout+=(QString(" 费用：%1").arg(section.price));
        detailRout+=('\n');

        if(section.end_time<section.start_time)//过夜了
        {
            day++;
            detailRout+=(QString("第%1天").arg(day)+'\n');
        }
        //停留时的状态
        curStatus.transport=((i+2)!=all.end())?"游玩中":"到达终点";
        curStatus.curCity=section.arrivecity;
        curStatus.startTime=section.end_time.hour()+double(section.end_time.minute())/60+day*24-iniTime;
        statuses.append(curStatus);

    }
    double time=day*24+statuses[statuses.size()-1].startTime-iniTime+24;
    if(time>limit)
        return false;
    else
        return true;
}
void clearPath(){
    for(int i=0;i<13;i++)
        totalPath[i].route.clear();
}
