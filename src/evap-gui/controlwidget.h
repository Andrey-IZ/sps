#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QTimer>
#include <QElapsedTimer>
#include <QWidget>
#include "../evap-protocol/interfaces/evapinterface.h"
#include "../evap/evapmessageservice.h"

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = 0);
    ~ControlWidget();

    void init(EvapInterface *evapInterface, EvapMessageService *ms);

signals:
    void resetMC();

public slots:
    void updateControlIndicatorsStatus();
    void updateControlFanSpeed();
    void updateFanSpeed();
    void startControl(int timeout);
    void stopControl();
    void initResetCmd();
    void initUI();

private slots:
    void sendTurnOnIndicators(bool checked);
    void on_PS_clicked(bool checked);
    void on_24v_clicked(bool checked);
    void on_12_1v_clicked(bool checked);
    void on_12_2v_clicked(bool checked);
    void on_pushButtonReset_clicked();
    void on_pushButtonSelfControl_clicked();
    void on_spinBoxLowerFans_valueChanged(int value);
    void on_spinBoxHigherFans_valueChanged(int value);

    void timeout();
    void on_pushButtonCalibrationADC_clicked();

private:
    Ui::ControlWidget *ui;
    QTimer timer;
    QElapsedTimer elTimer;
    int timeoutUpdate;
    bool timeoutStop;
    EvapInterface *_evapInterface;
    EvapMessageService *_ms;

    void initTableFans();
    void initConnect();
    void updateControls();
};

#endif // CONTROLWIDGET_H
