#include "route.h"
#include "ui_route.h"
#include <QPixmap>
#include <QDebug>

Route::Route(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Route)
{
    ui->setupUi(this);
    QPixmap pMap("://map");
//    qDebug()<<ui->label->size();
    pMap=pMap.scaled(QSize(967,750));
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

void Route::OpenMap()
{
    this->show();
}
