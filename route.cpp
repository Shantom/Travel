#include "route.h"
#include "ui_route.h"
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QPixmap>

Route::Route(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Route)
{
    ui->setupUi(this);
    QPixmap pMap("://map");
    //    pMap=pMap.scaled(QSize(967,750));
    pMap=pMap.scaled(QSize(1242,971));
    ui->label->setPixmap(pMap);
}

Route::~Route()
{
    delete ui;
}

void Route::on_pushButton_clicked()
{
    this->hide();
}

void Route::on_cityArrived(int city)
{
    visited.push_back(city);
    isStay=false;
    update();
}

void Route::on_wayPassing(int A, int B)
{
    passing.first=A;
    passing.second=B;
    isStay=false;
    update();
}

void Route::on_cityStaying(int city)
{
    visiting=city;
    isStay=true;
    update();
}

void Route::on_wayPassed(int A, int B)
{
    passed.append({A, B});
    update();
}

void Route::on_restart()
{
    visited.clear();
    visiting=-1;
    passed.clear();
    passing={-1,-1};
    isStay=true;
    repaint();
}

void Route::paintEvent(QPaintEvent *event)
{
    QPixmap *tmp=(QPixmap *)ui->label->pixmap();
    tmp->load("://map");
    QVector<QPoint> cityPoints={{912 ,370},{930, 398},{650, 645},
                                {1090, 200},{1020, 395},{885, 635},
                                {705, 430},{813, 370},{348, 248},
                                {930, 470},{750, 530},{1070, 770},{930, 600}};
    QPainter painter(tmp);
    QPen pen(Qt::black,10);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawPoints(cityPoints);//黑点表示未经过

    QVector<QLine> PassedLines;
    for(auto p:passed)
    {
        PassedLines.push_back({cityPoints.at(p.first),cityPoints.at(p.second)});
    }
    pen.setColor(Qt::blue);
    pen.setWidth(5);
    pen.setStyle(Qt::DashDotLine);
    painter.setPen(pen);
    if(!PassedLines.empty())
        painter.drawLines(PassedLines);//蓝线表示已经过


    if(isStay)//Point
    {
        if(visiting!=-1)
        {
            pen.setColor(Qt::green);
            pen.setWidth(8);
            painter.setPen(pen);
            painter.drawPoint(cityPoints.at(visiting));
        }
    }

    QVector<QPoint> VisitedPoints;
    for(auto i:visited)
        VisitedPoints.push_back(cityPoints.at(i));
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoints(VisitedPoints);//红点表示已经过

    if(isStay==0&&passing.first!=-1)//Line
    {
        pen.setColor(Qt::green);
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawLine(cityPoints.at(passing.first),cityPoints.at(passing.second));
    }


}

void Route::OpenMap()
{
    this->show();
}
