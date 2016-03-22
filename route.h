#ifndef ROUTE_H
#define ROUTE_H

#include <QWidget>

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

private:
    Ui::Route *ui;
};

#endif // ROUTE_H
