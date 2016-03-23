#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <passenger.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Passenger m_Psg;

signals:
    void SendOpen();

public slots:


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

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
