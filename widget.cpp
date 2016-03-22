#include "widget.h"
#include "passenger.h"
#include "ui_widget.h"
#include <QListWidgetItem>
#include <QRegExp>
QStringList cityList({"北京","天津","成都","哈尔滨","大连","威海",
                      "银川","呼和浩特","乌鲁木齐",
                      "济南","西安","台北","六安"});

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_Psg.setEnd("天津");
    m_Psg.setStart("北京");

    ui->checkBoxSequence->setChecked(true);//默认有顺序
    ui->listWidgetYet->addItems(cityList);
    ui->listWidgetYet->setCurrentRow(0);//默认选中第一行，防止未选择程序崩溃
//    ui->listWidgetSeleted->setSortingEnabled(true);
//    ui->listWidgetYet->setSortingEnabled(true);
    ui->comboBoxStart->addItems(cityList);
    QStringList tmp=cityList;
    tmp.removeFirst();
    ui->comboBoxEnd->addItems(tmp);
    //ui->doubleSpinBoxLimit->setEnabled(false);
    ui->doubleSpinBoxLimit->setSingleStep(0.5);//一步半小时
    ui->doubleSpinBoxLimit->setSuffix(" 小时");
    ui->radioButtonTimeFare->setChecked(true);//默认策略三
    ui->doubleSpinBoxStay->setSuffix(" 小时");
    ui->doubleSpinBoxStay->setSingleStep(0.5);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonShowMap_clicked()
{
    emit SendOpen();//释放显示地图信号
}

void Widget::on_pushButtonAdd_clicked()//按钮 <<
{
    if(ui->listWidgetYet->count()==0)
        return;//防止列表空程序崩溃
    double limitTime=ui->doubleSpinBoxStay->value();
    ui->doubleSpinBoxStay->setValue(0);//恢复为0
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

void Widget::on_comboBoxStart_currentTextChanged(const QString &arg1)
{
    //if(this->isVisible())
    {
        ui->listWidgetYet->insertItem(0,m_Psg.getStart());

        QList<QListWidgetItem *> preStart=ui->listWidgetSeleted->findItems(arg1,Qt::MatchStartsWith);
        if(!preStart.isEmpty())
           for (auto a:preStart)
                   delete a;
        preStart=ui->listWidgetYet->findItems(arg1,Qt::MatchStartsWith);
        if(!preStart.isEmpty())
            for (auto a:preStart)
                    delete a;
    }

    int curIndex=ui->comboBoxEnd->findText(arg1);//End中找到一样的
    ui->comboBoxEnd->removeItem(curIndex);//然后删掉
    if(this->isVisible())
        ui->comboBoxEnd->addItem(m_Psg.getStart());


    m_Psg.setStart(arg1);

}
void Widget::on_comboBoxEnd_currentTextChanged(const QString &arg1)
{
    //if(this->isVisible())
    {
        ui->listWidgetYet->insertItem(0,m_Psg.getEnd());

        QList<QListWidgetItem *> preEnd=ui->listWidgetSeleted->findItems(arg1,Qt::MatchStartsWith);
        if(!preEnd.isEmpty())
            for (auto a:preEnd)
                delete a;
        preEnd=ui->listWidgetYet->findItems(arg1,Qt::MatchStartsWith);
        if(!preEnd.isEmpty())
            for (auto a:preEnd)
                delete a;
    }

    int curIndex=ui->comboBoxStart->findText(arg1);//Start中找到一样的
    ui->comboBoxStart->removeItem(curIndex);//然后删掉
    if(this->isVisible())
        ui->comboBoxStart->addItem(m_Psg.getEnd());

    m_Psg.setEnd(arg1);
}
void Widget::on_radioButtonFare_clicked()
{
    m_Psg.setPolicy(Passenger::minTime);
    ui->doubleSpinBoxLimit->setEnabled(false);

}

void Widget::on_radioButtonTime_clicked()
{
    m_Psg.setPolicy(Passenger::minCost);
    ui->doubleSpinBoxLimit->setEnabled(false);
}

void Widget::on_radioButtonTimeFare_clicked()
{
    m_Psg.setPolicy(Passenger::timeLimitCost);
    ui->doubleSpinBoxLimit->setEnabled(true);

}

void Widget::on_checkBoxSequence_toggled(bool checked)
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

void Widget::on_doubleSpinBoxLimit_valueChanged(double arg1)//还没有卵用
{
    ui->label->setFocus();
}
