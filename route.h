#ifndef ROUTE_H
#define ROUTE_H

#include <QWidget>
#include <QVector>

namespace Ui {
class Route;
}

class Route : public QWidget
{
    Q_OBJECT

public:
    explicit Route(QWidget *parent = 0);
    ~Route();

public slots:
    void OpenMap();

private slots:
    void on_pushButton_clicked();
    void on_cityArrived(int);
    void on_wayPassing(int, int);
    void on_cityStaying(int);
    void on_wayPassed(int, int);
    void on_restart();

private:
    Ui::Route *ui;
    QVector<int> visited;
    int visiting=-1;
    QVector<QPair<int,int>> passed;
    QPair<int,int> passing={-1,-1};
    bool isStay=true;

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // ROUTE_H
