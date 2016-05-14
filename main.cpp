#include "widget.h"
#include "route.h"
#include <QApplication>
#include <QIcon>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setFixedSize(671,320);
    Route r;

    w.setWindowTitle("旅行模拟器");
    QIcon iconTravel("://mainIcon");
    w.setWindowIcon(iconTravel);
    r.setWindowTitle("旅行路线地图");
    QIcon iconMap("://MapIcon");
    r.setWindowIcon(iconMap);
    QWidget::connect(&w,SIGNAL(SendOpen()),&r,SLOT(OpenMap()));
    QWidget::connect(&w,SIGNAL(cityArrived(int)),&r,SLOT(on_cityArrived(int)));
    QWidget::connect(&w,SIGNAL(wayPassed(int,int)),&r,SLOT(on_wayPassed(int,int)));
    QWidget::connect(&w,SIGNAL(cityStaying(int)),&r,SLOT(on_cityStaying(int)));
    QWidget::connect(&w,SIGNAL(wayPassing(int,int)),&r,SLOT(on_wayPassing(int,int)));
    QWidget::connect(&w,SIGNAL(restartRoute()),&r,SLOT(on_restart()));

    w.show();

    return a.exec();
}
