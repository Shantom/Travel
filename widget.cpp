#include "widget.h"
#include "passenger.h"
#include "ui_widget.h"
#include "graph.h"
#include "timetable.h"
#include <QListWidgetItem>
#include <QRegExp>
#include <QMessageBox>
#include <QDebug>
#include <QMap>




Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
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

    ui->checkBoxSequence->setChecked(true);//默认有顺序
    ui->doubleSpinBoxLimit->setValue(10);//默认限时十个小时
    ui->comboBoxStart->addItems(cityList);
    QStringList tmp=cityList;
    tmp.removeFirst();
    ui->comboBoxEnd->addItems(tmp);
    ui->doubleSpinBoxLimit->setSingleStep(0.5);//一步半小时
    ui->doubleSpinBoxLimit->setSuffix(" 小时");
    ui->radioButtonFare->setChecked(true);//默认最少费用策略
    m_Psg.setPolicy(Passenger::minCost);
    ui->doubleSpinBoxLimit->setEnabled(false);
    ui->doubleSpinBoxStay->setSuffix(" 小时");
    ui->doubleSpinBoxStay->setSingleStep(0.5);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonShowMap_clicked()//点击 显示地图 按钮
{
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
}

void Widget::on_pushButtonUp_clicked()//按钮 ↑
{
    int curRow=ui->listWidgetSeleted->currentRow();
    QListWidgetItem *curItem=ui->listWidgetSeleted->takeItem(curRow);//先删除再插入到上一行
    ui->listWidgetSeleted->insertItem(curRow-1,curItem);
    ui->listWidgetSeleted->setCurrentItem(curItem);
}

void Widget::on_pushButtonDown_clicked()//按钮 ↓
{
    int curRow=ui->listWidgetSeleted->currentRow();
    QListWidgetItem *curItem=ui->listWidgetSeleted->takeItem(curRow);//先删除再插入到下一行
    ui->listWidgetSeleted->insertItem(curRow+1,curItem);
    ui->listWidgetSeleted->setCurrentItem(curItem);

}

void Widget::on_comboBoxStart_currentTextChanged(const QString &arg1)//起点改变
{
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
    m_Psg.setPolicy(Passenger::minCost);
    ui->doubleSpinBoxLimit->setEnabled(false);
}

void Widget::on_radioButtonTime_clicked()//选中 最短时间
{
    m_Psg.setPolicy(Passenger::minTime);
    ui->doubleSpinBoxLimit->setEnabled(false);
}

void Widget::on_radioButtonTimeFare_clicked()//选中 限时最短时间
{
    m_Psg.setPolicy(Passenger::timeLimitCost);
    ui->doubleSpinBoxLimit->setEnabled(true);

}

void Widget::on_checkBoxSequence_toggled(bool checked)//点击 是否有顺序的复选框
{
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
    m_Psg.setLimitTime(arg1);
}

void Widget::on_pushButtonStart_clicked()//点击开始按钮
{

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
    //stayTime.append(0);
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
        if(m_Psg.getPolicy()==Passenger::minCost)
        {

            QString detailRout=getRouteString_MinCost(m_Psg,stayTime);
            QMessageBox::information(this,"路线",detailRout);
        }
    }
}

/*计算出需要打印出来的字符串*/
QString Widget::getRouteString_MinCost(Passenger Psg,QList<double> stayTime)
{
    Graph G;
    G.CreateGraph_MinCost();
    vector<QString> route;
    vector<QString> midCities;

    auto tmpWayCities=Psg.getWayCities();
    for(auto a:tmpWayCities)
    {
        midCities.push_back(a.first);
    }

    int cost=G.LeastCost(Psg.getStart(),Psg.getEnd(),midCities,Psg.isSequence(),route);

    QString detailRout;

    QTime preArriTime(23,59,59);
    int day=0;
    size_t j=0;//停留时间的标尺
    for(auto i=route.begin();i<route.end()-1;++i)
    {
        Info section=TimeTable::getInfo_MinCost(*i,*(i+1));

        QTime tmp;
        if(j<midCities.size()&&midCities.at(j)==*i)
        {
            tmp=preArriTime;
            preArriTime=preArriTime.addSecs(3600*stayTime.at(j++));//加上停留时间
        }
        if(preArriTime>section.departtime||preArriTime<tmp)//判断是否需要第二天再走
        {
            day++;
            preArriTime=section.arrivetime;
            detailRout+=(tr("第%1天").arg(day)+'\n');
        }

        if(section.arrivetime<section.departtime)//过夜了
            day++;

        detailRout+=(section.trainnumber+' ');
        detailRout+=(section.departcity+tr("到")+section.arrivecity+' ');
        detailRout+=(section.departtime.toString("HH:mm")+tr("到")+section.arrivetime.toString("HH:mm"));
        detailRout+=(tr(" 费用：%1").arg(section.price));
        detailRout+=('\n');
    }

    detailRout+=(Widget::tr("总费用:%1").arg(cost));

    return detailRout;
}

void Widget::on_checkBoxCycle_toggled(bool checked)//点击 是否Cycle 复选框
{
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
