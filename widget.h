#ifndef WIDGET_H
#define WIDGET_H

#include "timer.h"
#include <QWidget>
#include <passenger.h>
#include <QMap>
#include <QListWidgetItem>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QString getRouteString_MinCost(Passenger Psg, QList<double> stayTime,QList<Status> &statuses);

private:
    Passenger m_Psg;

signals:
    void SendOpen();
    void cityArrived(int);
    void wayPassing(int,int);
    void cityStaying(int);
    void wayPassed(int,int);
    void restartRoute();

public slots:

    void RecvTimerStart();

    void RecvTimerStop(bool isPaused);

    void RecvTimerTick(int time);


private slots:

    void on_pushButtonShowMap_clicked();

    void on_pushButtonAdd_clicked();

    void on_pushButtonRemove_clicked();

    void on_pushButtonUp_clicked();

    void on_pushButtonDown_clicked();

    void on_comboBoxStart_currentTextChanged(const QString &arg1);

    void on_radioButtonFare_clicked();

    void on_radioButtonTime_clicked();

    void on_radioButtonTimeFare_clicked();

    void on_checkBoxSequence_toggled(bool checked);

    void on_doubleSpinBoxLimit_valueChanged(double arg1);

    void on_comboBoxEnd_currentTextChanged(const QString &arg1);

    void on_pushButtonStart_clicked();

    void on_checkBoxCycle_toggled(bool checked);

    void on_pushButtonAbout_clicked();

    void on_pushButtonPause_clicked();

    void on_pushButtonRestart_clicked();

private:
    Ui::Widget *ui;
    QStringList cityList;
    QMap<QString,int> cityToInt;
    QList<QListWidgetItem*> itemList;
    QList<Status> statuses;
    double iniTime;//with an extra 24 hours(i've forgot why)
    int days;
    Timer *m_timer;

};

#endif // WIDGET_H
