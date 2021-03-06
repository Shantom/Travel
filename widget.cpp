#include "widget.h"
#include "passenger.h"
#include "ui_widget.h"
#include "graph.h"
#include "timetable.h"
#include "timer.h"
#include "timelimitcheapest.h"
#include <QListWidgetItem>
#include <QRegExp>
#include <QMessageBox>
#include <QDebug>
#include <QMap>
#include <fstream>
#define swtName(city) if(city=="尔滨") city="哈尔滨"; else if(city=="浩特") city="呼和浩特"; \
    else if(city=="木齐") city="乌鲁木齐"
#define swtName_2(city) if(city=="哈尔") city="哈尔滨"; else if(city=="呼和") city="呼和浩特"; \
    else if(city=="乌鲁") city="乌鲁木齐"



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    logFile.open("/home/shantom/Desktop/travel.log",ios_base::app);

    cityList.append({"北京","天津","成都","哈尔滨","大连","武汉",
                     "银川","呼和浩特","乌鲁木齐",
                     "济南","西安","台北","六安"});
    TimeTable T;

    int i=0;
    for(auto a:cityList)
    {
        cityToInt[a]=i++;
        itemList.append(new QListWidgetItem(a));
    }
    for(auto a:itemList)
    {
        ui->listWidgetYet->addItem(a);
    }
    ui->listWidgetYet->setCurrentRow(0);//默认选中第一行，防止未选择程序崩溃


    m_Psg.setStart(cityList.at(0));
    m_Psg.setEnd(cityList.at(1));
    ui->radioButtonFare->setChecked(true);

    ui->checkBoxSequence->setChecked(false);//默认没有顺序
    on_checkBoxSequence_toggled(false);
    ui->comboBoxStart->addItems(cityList);
    QStringList tmp=cityList;
    tmp.removeFirst();
    ui->comboBoxEnd->addItems(tmp);
    ui->doubleSpinBoxLimit->setSingleStep(0.5);//一步半小时
    ui->doubleSpinBoxLimit->setSuffix(" 小时");
    ui->doubleSpinBoxLimit->setMaximum(999);
    ui->doubleSpinBoxLimit->setValue(999);//默认限时十个小时
    ui->radioButtonFare->setChecked(true);//默认最少费用策略
    m_Psg.setPolicy(Passenger::minCost);
    ui->doubleSpinBoxLimit->setEnabled(false);
//    ui->doubleSpinBoxLimit->set
    ui->doubleSpinBoxStay->setSuffix(" 小时");
    ui->doubleSpinBoxStay->setSingleStep(0.5);
    m_timer=nullptr;
    ui->pushButtonPause->setEnabled(false);
    ui->lcdNumberTime->display("0:00");
    days=1;

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonShowMap_clicked()//点击 显示地图 按钮
{
    logFile<<"显示地图"<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
    emit SendOpen();//释放显示地图信号
}

void Widget::on_pushButtonAdd_clicked()//按钮 <<
{
    if(ui->listWidgetYet->count()==0)
        return;//防止列表空程序崩溃
    double limitTime=ui->doubleSpinBoxStay->value();
    ui->doubleSpinBoxStay->setValue(0);//停留时间的框框恢复为0
    int curRow=ui->listWidgetYet->currentRow();
    QListWidgetItem *curItem=ui->listWidgetYet->takeItem(curRow);//从右侧列表删除并保存指针
    curItem->setText(curItem->text()+=QString("(%1)").arg(limitTime));
    ui->listWidgetSeleted->addItem(curItem);//添加到左侧列表
    ui->listWidgetSeleted->setCurrentItem(curItem);//默认选中刚添加的这一行防崩溃
    logFile<<"按钮 <<:"<<(ui->listWidgetSeleted->currentItem()->text().toStdString())<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
}

void Widget::on_pushButtonRemove_clicked()//按钮 >>
{
    if(ui->listWidgetSeleted->count()==0)
        return;//防止列表空程序崩溃
    int curRow=ui->listWidgetSeleted->currentRow();    
    QListWidgetItem *curItem=ui->listWidgetSeleted->takeItem(curRow);
    curItem->setText(curItem->text().remove(QRegExp("\\(\\d*\\.?\\d*\\)")));//删除括号数字
    ui->listWidgetYet->insertItem(0,curItem);//插入到第一行
    ui->listWidgetYet->setCurrentItem(curItem);
    logFile<<"按钮 >>:"<<ui->listWidgetYet->currentItem()->text().toStdString()<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
}

void Widget::on_pushButtonUp_clicked()//按钮 ↑
{
    int curRow=ui->listWidgetSeleted->currentRow();
    logFile<<"按钮 ↑:"<<ui->listWidgetSeleted->item(curRow)->text().toStdString()<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
    QListWidgetItem *curItem=ui->listWidgetSeleted->takeItem(curRow);//先删除再插入到上一行
    ui->listWidgetSeleted->insertItem(curRow-1,curItem);
    ui->listWidgetSeleted->setCurrentItem(curItem);
}

void Widget::on_pushButtonDown_clicked()//按钮 ↓
{
    int curRow=ui->listWidgetSeleted->currentRow();
    logFile<<"按钮 ↓:"<<ui->listWidgetSeleted->item(curRow)->text().toStdString()<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
    QListWidgetItem *curItem=ui->listWidgetSeleted->takeItem(curRow);//先删除再插入到下一行
    ui->listWidgetSeleted->insertItem(curRow+1,curItem);
    ui->listWidgetSeleted->setCurrentItem(curItem);

}

void Widget::on_comboBoxStart_currentTextChanged(const QString &arg1)//起点改变
{
//    days=1;//一旦修改了开始城市，就代表了重开
    logFile<<"起点修改:"<<arg1.toStdString()<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;

    /*对途经城市框进行适配*/
    itemList[cityToInt[arg1]]->setText(itemList[cityToInt[arg1]]->text().remove(QRegExp("\\(\\d*\\.?\\d*\\)")));//删除括号数字
    ui->listWidgetYet->insertItem(cityToInt[m_Psg.getStart()],itemList[cityToInt[m_Psg.getStart()]]);//恢复之前的起点
    ui->listWidgetSeleted->takeItem(ui->listWidgetSeleted->row(itemList[cityToInt[arg1]]));//删除左侧
    ui->listWidgetYet->takeItem(ui->listWidgetYet->row(itemList[cityToInt[arg1]]));//删除右侧

    if(!ui->checkBoxCycle->isChecked())//起点终点不一样
    {
        /*对终点下拉框进行适配*/
        int curIndex=ui->comboBoxEnd->findText(arg1);//End中找到一样的
        ui->comboBoxEnd->removeItem(curIndex);//然后删掉
        if(this->isVisible())
            ui->comboBoxEnd->addItem(m_Psg.getStart());

    }

    m_Psg.setStart(arg1);
    if(ui->checkBoxCycle->isChecked())//起点终点一样
        m_Psg.setEnd(arg1);

}

void Widget::on_comboBoxEnd_currentTextChanged(const QString &arg1)//终点改变
{
    logFile<<"终点修改:"<<arg1.toStdString()<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;

    /*对途经城市框进行适配*/
    itemList[cityToInt[arg1]]->setText(itemList[cityToInt[arg1]]->text().remove(QRegExp("\\(\\d*\\.?\\d*\\)")));//删除括号数字
    ui->listWidgetYet->insertItem(cityToInt[m_Psg.getEnd()], itemList[cityToInt[m_Psg.getEnd()]]);//恢复之前的终点
    ui->listWidgetSeleted->takeItem(ui->listWidgetSeleted->row(itemList[cityToInt[arg1]]));//删除右侧
    ui->listWidgetYet->takeItem(ui->listWidgetYet->row(itemList[cityToInt[arg1]]));//删除左侧

    /*对终点下拉框进行适配*/
    int curIndex=ui->comboBoxStart->findText(arg1);//Start中找到一样的
    ui->comboBoxStart->removeItem(curIndex);//然后删掉
    if(this->isVisible())
        ui->comboBoxStart->addItem(m_Psg.getEnd());

    m_Psg.setEnd(arg1);
}

void Widget::on_radioButtonFare_clicked()//选中 最少费用
{
    logFile<<"策略修改:"<<"最少费用"<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;

    m_Psg.setPolicy(Passenger::minCost);
    ui->doubleSpinBoxLimit->setEnabled(false);
}

void Widget::on_radioButtonTime_clicked()//选中 最短时间
{
    logFile<<"策略修改:"<<"最短时间"<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;

    m_Psg.setPolicy(Passenger::minTime);
    ui->doubleSpinBoxLimit->setEnabled(false);
}

void Widget::on_radioButtonTimeFare_clicked()//选中 限时最短时间
{
    logFile<<"策略修改:"<<"限时最短时间"<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;

    m_Psg.setPolicy(Passenger::timeLimitCost);
    ui->doubleSpinBoxLimit->setEnabled(true);

}

void Widget::on_checkBoxSequence_toggled(bool checked)//点击 是否有顺序的复选框
{
    logFile<<"有顺序的复选框修改:"<<(checked?"是":"否")<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;

    m_Psg.setSequence(checked);

    if(checked)
    {
        ui->pushButtonUp->setEnabled(true);
        ui->pushButtonDown->setEnabled(true);
    }
    else
    {
        ui->pushButtonUp->setEnabled(false);
        ui->pushButtonDown->setEnabled(false);
    }
}

void Widget::on_doubleSpinBoxLimit_valueChanged(double arg1)//限时最短时间数值修改
{
    logFile<<"限时最短时间数值修改:"<<arg1<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;

    m_Psg.setLimitTime(arg1);
}

void Widget::on_pushButtonStart_clicked()//点击开始按钮
{
    logFile<<"点击开始"<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;

    if(days==1)
        emit restartRoute();
    QString plcy[3]={"最少费用","最短时间","限时最少费用"};
    QString strResult;

    strResult+=(tr("起点：")+m_Psg.getStart()+"\n");
    strResult+=(tr("终点：")+m_Psg.getEnd()+"\n");
    strResult+=(tr("策略：")+plcy[m_Psg.getPolicy()]+"\n");
    if(m_Psg.getPolicy()==Passenger::timeLimitCost)
        strResult+=(tr("限时：%1小时").arg(m_Psg.getLimitTime())+"\n");
    strResult+=(tr("途经城市：")+(m_Psg.isSequence()?
                                 tr("（有顺序）"):tr("（无顺序）"))+"\n\n");
    QRegExp numModel("\\d+\\.?\\d*");//不带括号
    QRegExp numModelWithPara("\\(\\d+\\.?\\d*\\)");//带括号

    QList<double> stayTime;
    if(ui->listWidgetSeleted->count()==0)//如果没有途经城市
    {
        if(ui->checkBoxCycle->isChecked())//如果起点终点一样而且没有途经城市
        {
            QMessageBox::critical(this,tr("错误"),tr("没有选择途经城市"));
            return;
        }
        strResult+="无\n";
        m_Psg.setWayCities(QList<QPair<QString, double>>());
    }

    else//有途径城市
    {
        QList<QPair<QString, double>> tmp;

        for(int i=0;i<ui->listWidgetSeleted->count();++i)
        {
            QString name=ui->listWidgetSeleted->item(i)->text().remove(numModelWithPara);//提取地名
            numModel.indexIn(ui->listWidgetSeleted->item(i)->text());
            stayTime.append(numModel.cap().toDouble());//提取数字
            tmp.append(QPair<QString, double>(name,stayTime.at(i)));
            m_Psg.setWayCities(tmp);
            strResult+=name+tr(" (%1时)").arg(stayTime.at(i))+"\n";
        }
    }

    QMessageBox message(QMessageBox::Question,"旅客信息",strResult,QMessageBox::Yes|QMessageBox::No);
    message.exec();
    if(message.clickedButton()==message.button(QMessageBox::Yes))
    {
        logFile<<"旅客信息:\n"<<strResult.toStdString()<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
        QString detailRout;
        if(m_Psg.getPolicy()==Passenger::minCost)
        {
            detailRout=getRouteString_MinCost(m_Psg,stayTime,statuses);
        }
        else if(m_Psg.getPolicy()==Passenger::minTime)
        {
            detailRout=getRouteString_MinCost(m_Psg,stayTime,statuses);
        }
        else
        {
            TimeLimitCheapest TG(m_Psg);
            if(!TG.AStar(statuses,detailRout))
            {
                QMessageBox::information(this,"未找到","未找到路线");//
                return;
               }

            iniTime=13+24;
        }
        iniTime+=(days-1)*24;//for paused timer
        QMessageBox::information(this,"路线",detailRout);//
        if(!m_timer)
            delete m_timer;
        m_timer=new Timer(2*((statuses.end()-1)->startTime));
        connect(m_timer,SIGNAL(timerStart()),this,SLOT(RecvTimerStart()));
        connect(m_timer,SIGNAL(timerStopped(bool)),this,SLOT(RecvTimerStop(bool)));
        connect(m_timer,SIGNAL(timerTick(int)),this,SLOT(RecvTimerTick(int)));
        ui->lcdNumberTime->display(QString("%1:00").arg(iniTime-24));
        m_timer->StartTimer();
    }
    else
    {
        logFile<<"点击取消"<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
    }
}

/*计算出需要打印出来的字符串*/
QString Widget::getRouteString_MinCost(Passenger Psg, QList<double> stayTime,
                                       QList<Status> &statuses)
{
    Graph G;
    G.CreateGraph();
    vector<QString> route;
    vector<QString> midCities;

    auto tmpWayCities=Psg.getWayCities();
    for(auto a:tmpWayCities)
    {
        midCities.push_back(a.first);
    }
    int cost;
    double time;
    vector<EdgeType> edgeList;
    if(Psg.getPolicy()==Passenger::minCost)
        cost=G.LeastCost(Psg.getStart(),Psg.getEnd(),midCities,Psg.isSequence(),route);
    else if(Psg.getPolicy()==Passenger::minTime)
        time=G.LeastTime(0,Psg.getStart(),Psg.getEnd(),midCities,Psg.isSequence(),route,edgeList);

    QString detailRout;

    QTime preArriTime(23,59,59);//上一个城市的到达时间
    int day=0;
    size_t j=0;//停留时间的标尺

    EdgeType iniInfo;
    if(Psg.getPolicy()==Passenger::minCost)
        iniInfo=G.getInfo_MinCost(route[0],route[1]);
    else if(Psg.getPolicy()==Passenger::minTime)
        iniInfo=G.getInfo_MinTime(route[0],route[1],preArriTime);

    iniTime=iniInfo.start_time.hour()+double(iniInfo.start_time.minute())/60+24;

    for(auto i=route.begin();i<route.end()-1;++i)//route保存包括起点终点的所有城市名
    {


        QTime tmp=preArriTime;
        if(j<midCities.size()&&midCities.at(j)==*i)//如果当前城市为停留城市
        {
            preArriTime=preArriTime.addSecs(3600*stayTime.at(j++));//加上停留时间
        }

        EdgeType section;
        if(Psg.getPolicy()==Passenger::minCost)
            section=TimeTable::getInfo_MinCost(*i,*(i+1));
        else if(Psg.getPolicy()==Passenger::minTime)
            section=G.getInfo_MinTime(*i,*(i+1),preArriTime);


        if(preArriTime>section.start_time||preArriTime<tmp)//判断是否需要第二天再走
        {
            day++;
            detailRout+=(tr("第%1天").arg(day)+'\n');
        }

        Status curStatus;
        //坐车时的状态
        curStatus.transport=section.trainnumber;
        curStatus.curCity=section.departcity+'-'+section.arrivecity;
        curStatus.startTime=section.start_time.hour()+double(section.start_time.minute())/60+24*day-iniTime;
        statuses.append(curStatus);

        preArriTime=section.end_time;

        detailRout+=(section.trainnumber+' ');
        detailRout+=(section.departcity+tr("到")+section.arrivecity+' ');
        detailRout+=(section.start_time.toString("HH:mm")+tr("到")+section.end_time.toString("HH:mm"));
        detailRout+=(tr(" 费用：%1").arg(section.price));
        detailRout+=('\n');

        if(section.end_time<section.start_time)//过夜了
        {
            day++;
            detailRout+=(tr("第%1天").arg(day)+'\n');
        }
        //停留时的状态
        curStatus.transport=((i+2)!=route.end())?"游玩中":"到达终点";
        curStatus.curCity=section.arrivecity;
        curStatus.startTime=section.end_time.hour()+double(section.end_time.minute())/60+day*24-iniTime;
        statuses.append(curStatus);

    }
    if(Psg.getPolicy()==Passenger::minCost)
        detailRout+=(Widget::tr("总费用:%1").arg(cost));
    else if(Psg.getPolicy()==Passenger::minTime)
        detailRout+=(Widget::tr("总时间:%1").arg(time-iniTime+24));


    return detailRout;
}

void Widget::on_checkBoxCycle_toggled(bool checked)//点击 是否Cycle 复选框
{
    logFile<<(checked?"Cycle":"取消Cycle")<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;

    ui->comboBoxEnd->setEnabled(!checked);//禁用终点框

    if(checked)//起点终点相同，即禁用状态
    {
        QString curEnd=ui->comboBoxEnd->currentText();
        ui->listWidgetYet->insertItem(0,itemList[cityToInt[curEnd]]);
        m_Psg.setEnd(m_Psg.getStart());
    }
    else
    {
        QString curEnd=ui->comboBoxEnd->currentText();
        m_Psg.setEnd(curEnd);
        itemList[cityToInt[curEnd]]->setText(itemList[cityToInt[curEnd]]->text().remove(QRegExp("\\(\\d*\\.?\\d*\\)")));//删除括号数字
        ui->listWidgetSeleted->takeItem(ui->listWidgetSeleted->row(itemList[cityToInt[curEnd]]));//删除右侧
        ui->listWidgetYet->takeItem(ui->listWidgetYet->row(itemList[cityToInt[curEnd]]));//删除左侧
    }
}

void Widget::on_pushButtonAbout_clicked()
{
    QString strAbout="本程序为算法与数据结构的课程设计\n";
    strAbout+="参与人员：\nShantom\nrartxt\n";
    strAbout+="联系方式：\nsalpha1345@gmail.com\n";
    strAbout+="源代码：\nhttps://github.com/Shantom/Travel";
    QMessageBox::about(this,"关于本程序",strAbout);
}

void Widget::RecvTimerStart()
{
    ui->checkBoxCycle->setEnabled(false);
    ui->checkBoxSequence->setEnabled(false);
    ui->comboBoxStart->setEnabled(false);
    ui->comboBoxEnd->setEnabled(false);
    ui->doubleSpinBoxLimit->setEnabled(false);
    ui->doubleSpinBoxStay->setEnabled(false);
    ui->pushButtonAdd->setEnabled(false);
    ui->pushButtonDown->setEnabled(false);
    ui->pushButtonRemove->setEnabled(false);
    ui->pushButtonUp->setEnabled(false);
    ui->pushButtonStart->setEnabled(false);
    ui->radioButtonFare->setEnabled(false);
    ui->radioButtonTime->setEnabled(false);
    ui->radioButtonTimeFare->setEnabled(false);
    ui->pushButtonPause->setEnabled(true);
}

void Widget::RecvTimerStop(bool isPaused)
{

    ui->checkBoxCycle->setEnabled(true);
    ui->checkBoxSequence->setEnabled(true);
    if(m_Psg.getPolicy()==Passenger::timeLimitCost)
        ui->doubleSpinBoxLimit->setEnabled(true);
    if(!ui->checkBoxCycle->isChecked())
        ui->comboBoxEnd->setEnabled(true);
    ui->doubleSpinBoxStay->setEnabled(true);
    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonDown->setEnabled(true);
    ui->pushButtonRemove->setEnabled(true);
    ui->pushButtonUp->setEnabled(true);
    ui->radioButtonFare->setEnabled(true);
    ui->radioButtonTime->setEnabled(true);
    ui->radioButtonTimeFare->setEnabled(true);
    for(int i=0;i<14;++i)
        on_pushButtonRemove_clicked();

    if(isPaused)
    {
        QString curSta=ui->labelCurStatus->text();
        QString curCity=QString(curSta[curSta.size()-2])+QString(curSta[curSta.size()-1]);
        swtName(curCity);

        ui->comboBoxStart->setCurrentText(curCity);
        ui->pushButtonPause->setEnabled(true);
    }
    else
    {
        ui->pushButtonStart->setEnabled(true);
        ui->comboBoxStart->setEnabled(true);
        ui->pushButtonPause->setEnabled(false);
        QMessageBox::information(this,"结束","旅途已结束");
        emit restartRoute();
    }
}

void Widget::RecvTimerTick(int time)
{
    QString preStatus=ui->labelCurStatus->text();
    for(auto a:statuses)
    {
        if(time*0.5>=a.startTime)
        {
            ui->labelCurStatus->setText(a.transport+' '+a.curCity);
        }
    }
    QString curStatus=ui->labelCurStatus->text();

    if(curStatus!=preStatus)//画图以及日志
    {

        logFile<<curStatus.toStdString()<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
        if(preStatus.startsWith("游玩中")||preStatus=="无"||preStatus.startsWith("到达终点"))
        {
            if(preStatus!="无"&&!preStatus.startsWith("到达终点"))
            {
                QString preCity=QString(preStatus[preStatus.size()-2])+QString(preStatus[preStatus.size()-1]);
                swtName(preCity);
                emit cityArrived(cityToInt[preCity]);
            }
            QString ACity=QString(curStatus.at(4))+QString(curStatus.at(5));
            QString BCity=QString(curStatus.at(curStatus.size()-2))+QString(curStatus.at(curStatus.size()-1));
            swtName(BCity);
            swtName_2(ACity);
            emit wayPassing(cityToInt[ACity],cityToInt[BCity]);
        }
        else
        {
            if(preStatus!="无"&&!preStatus.startsWith("到达终点"))
            {
                QString ACity=QString(preStatus.at(4))+QString(preStatus.at(5));
                QString BCity=QString(preStatus.at(preStatus.size()-2))+QString(preStatus.at(preStatus.size()-1));
                swtName(BCity);
                swtName_2(ACity);
                emit wayPassed(cityToInt[ACity],cityToInt[BCity]);
            }
            QString curCity=QString(curStatus[curStatus.size()-2])+QString(curStatus[curStatus.size()-1]);
            swtName(curCity);
            emit cityStaying(cityToInt[curCity]);
        }
    }

    double curTime=iniTime-24+time*0.5;
    int hour=curTime;
    int minute=(curTime-double(hour))*60;
    if(hour>=24)
    {
        days=hour/24+1;
        hour%=24;
    }
    ui->labelDays->setText(QString("第%1天").arg(days));
    ui->lcdNumberTime->display(QString("%1:%2%3").arg(hour).arg(minute/10).arg(0));
}

void Widget::on_pushButtonPause_clicked()
{
    logFile<<"已暂停"<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
    m_timer->isPaused=!m_timer->isPaused;
    if(m_timer->isPaused)
    {
        m_timer->StopTimer(true);//pause
        ui->pushButtonPause->setText("继续");
    }
    else
    {
        on_pushButtonStart_clicked();
        ui->pushButtonPause->setText("暂停");
    }
}

void Widget::on_pushButtonRestart_clicked()
{
    clearPath();
    logFile<<"已重开"<<"\t"<<QTime::currentTime().toString().toStdString()<<endl;
    days=1;//
    if(m_timer)
    {
        m_timer->StopTimer(false);
        delete m_timer;
        m_timer=nullptr;
    }
    ui->labelCurStatus->setText("无");
    ui->labelDays->setText("第N/A天");
    ui->comboBoxStart->setCurrentText("北京");
    ui->comboBoxEnd->setCurrentText("天津");
    ui->checkBoxCycle->setChecked(false);
    ui->lcdNumberTime->display("0:00");
    emit restartRoute();
}
