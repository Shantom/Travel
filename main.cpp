#include "widget.h"
#include "route.h"
#include <QApplication>
#include <QIcon>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    Route r;

    w.setWindowTitle("旅行模拟器");
    QIcon iconTravel("://mainIcon");
    w.setWindowIcon(iconTravel);
    r.setWindowTitle("旅行路线地图");
    QIcon iconMap("://MapIcon");
    r.setWindowIcon(iconMap);
    QWidget::connect(&w,SIGNAL(SendOpen()),&r,SLOT(OpenMap()));


    w.show();

    return a.exec();
}
