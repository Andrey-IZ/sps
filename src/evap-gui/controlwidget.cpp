#include "controlwidget.h"
#include "ui_controlwidget.h"
#include <QCheckBox>
#include <QDebug>
#include <QFile>
#include "../gui-lib/utilsforgui.h"
#include "../gui-lib/component/ledlabel.h"
#include "../evap-protocol/msgsetturnonps.h"
#include "../evap-protocol/msgsetturnonps_24v.h"
#include "../evap-protocol/msgsetturnon_12v_1.h"
#include "../evap-protocol/msgsetturnon_12v_2.h"
#include "../evap-protocol/msgturnonindicators.h"
#include "../evap-protocol/msgmicrocontrollerreboot.h"
#include "../evap-protocol/msgselfcontrol.h"
#include "../evap-protocol/msgsetfanspeed.h"
#include "../evap-protocol/msggetfanspeedcontrol.h"
#include "../evap-protocol/msggetfanspeed.h"
#include "../evap-protocol/msgreadindicatorstatus.h"
#include "../evap-protocol/msgcalibrationadc.h"
#include "../evap-protocol/msggetdeterminedfanspeed.h"


ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget), timeoutUpdate(500), timeoutStop(true)
{
    ui->setupUi(this);

    initConnect();
    initTableFans();
    initUI();
}

ControlWidget::~ControlWidget()
{
    timeoutStop = true;
    delete ui;
}

void ControlWidget::updateControls()
{
    updateFanSpeed();
    updateControlFanSpeed();
    updateControlIndicatorsStatus();
}

void ControlWidget::timeout()
{
    Q_ASSERT(_evapInterface != NULL);
    timer.stop();
    elTimer.restart();

    updateControls();

    int time = timeoutUpdate - int(elTimer.elapsed());
    qDebug() << "timeVentilation=" << time;
    if (!timeoutStop)
        timer.start(time > 0? time: 0);
}

void ControlWidget::updateControlIndicatorsStatus()
{
    MsgReadIndicatorStatus msg(_evapInterface);
    if(!_ms->getAsync(&msg)) {
        UtilsForGui::setEnabledAllWidgets(ui->groupBox_IndicatorSate, false, false);
        ui->labelUpperAuto->setText("(?)");
        ui->labelLowerAuto->setText("(?)");
        return;
    }

    ui->labelUpperAuto->setText(msg.isManualUpperFanSpeed? "":tr("(авто)"));
    ui->labelLowerAuto->setText(msg.isManualLowerFanSpeed? "":tr("(авто)"));

    UtilsForGui::setEnabledAllWidgets(ui->groupBox_IndicatorSate, true, false);

    ui->label_Indicators_Power->setLedOn(msg.power);
    ui->label_Indicators_Module->setLedOn(msg.module);
    ui->label_Indicators_isReady->setLedOn(msg.isReady);
    ui->label_Indicators_Ventilation->setLedOn(msg.ventilation);
    ui->label_Indicators_SP->setLedOn(msg.turnOnPowerSupply);
    ui->label_Indicators_24V->setLedOn(msg.turnOn24v);
    ui->label_Indicators_12v1->setLedOn(msg.turnOn12v_1);
    ui->label_Indicators_12v2->setLedOn(msg.turnOn12v_2);
}

void ControlWidget::updateControlFanSpeed()
{
    MsgGetFanSpeed msgFanSpeed(_evapInterface);
    if(_ms->getAsync(&msgFanSpeed)) {
        MsgGetFanSpeedControl msgFanSpeedControl(_evapInterface);
        if (_ms->getAsync(&msgFanSpeedControl))
        {
            QRegExp reUpper("label_fan_higher_(\\d)");
            QRegExp reLocal("label_fan_local_(\\d)");
            QRegExp reLower("label_fan_lower_(\\d)");

            foreach (LedLabel* ll, ui->groupBox_FanSpeedMonitor->findChildren<LedLabel*>()) {
                if (reUpper.indexIn(ll->objectName(), 0) != -1) {
                    int num = reUpper.cap(1).toInt() - 1;
                    int speedValue = int(msgFanSpeed.upper_compartment_fan.at(num));
                    QString speedValueStr = QString::number(speedValue);
                    ll->setText(speedValueStr);
                    bool isAcceptable = msgFanSpeedControl.upper_compartment_fans.at(num);
                    ll->setLedOn(isAcceptable);

                    if (!isAcceptable)
                        ll->setToolTip("скорость вентилятора ниже допустимого порога");
                    else
                        ll->setToolTip("скорость вращения вентиляторов (RPM) верхнего отсека");
                }
                if (reLocal.indexIn(ll->objectName(), 0) != -1) {
                    int num = reLocal.cap(1).toInt() - 1;
                    QString speedValue = QString::number(msgFanSpeed.local_compartment_fan.at(num));
                    ll->setText(speedValue);
                    bool isAcceptable = msgFanSpeedControl.local_compartment_fans.at(num);
                    ll->setLedOn(isAcceptable);
                    if (!isAcceptable)
                        ll->setToolTip("скорость вентилятора ниже допустимого порога");
                    else
                        ll->setToolTip("скорость вращения локальных вентиляторов (RPM)");
                }
                if (reLower.indexIn(ll->objectName(), 0) != -1) {
                    int num = reLower.cap(1).toInt() - 1;
                    int speedValue = int(msgFanSpeed.lower_compartment_fan.at(num));
                    QString speedValueStr = QString::number(speedValue);
                    ll->setText(speedValueStr);
                    bool isAcceptable = msgFanSpeedControl.lower_compartment_fans.at(num);
                    ll->setLedOn(isAcceptable);

                    if (!isAcceptable)
                        ll->setToolTip("скорость вентилятора ниже допустимого порога");
                    else
                        ll->setToolTip("скорость вращения вентиляторов (RPM) нижнего отсека");
                }
            }

            return;
        }
    }
    foreach (LedLabel* ll, ui->groupBox_FanSpeedMonitor->findChildren<LedLabel*>()) {
        ll->setText("-");
        ll->setState(LedLabel::Disabled);
    }
}

void ControlWidget::updateFanSpeed()
{
    MsgGetDeterminedFanSpeed msg(_evapInterface);

    if (_ms->getAsync(&msg)) {
        ui->spinBoxLowerFans->setValue(int(msg.lowerFanSpeed));
        ui->spinBoxHigherFans->setValue(int(msg.upperFanSpeed));
        return;
    } else {
        ui->spinBoxLowerFans->setValue(0);
        ui->spinBoxHigherFans->setValue(0);
    }
}

void ControlWidget::startControl(int timeout)
{
    timeoutStop = false;
    timeoutUpdate = timeout;
    timer.start(0);
}

void ControlWidget::stopControl()
{
    timeoutStop = true;
}

void ControlWidget::on_PS_clicked(bool checked)
{
    MsgSetTurnOnPS msg(_evapInterface);
    msg.isOn = !checked;
    if (!_ms->postAsync(&msg)) {
        static_cast<LedLabel*>(sender())->setState(LedLabel::Disabled);
        return;
    }
    updateControlIndicatorsStatus();
}

void ControlWidget::on_24v_clicked(bool checked)
{
    MsgSetTurnOnPS_24v msg(_evapInterface);
    msg.isOn = !checked;
    if (!_ms->postAsync(&msg)) {
        static_cast<LedLabel*>(sender())->setState(LedLabel::Disabled);
        return;
    }
    updateControlIndicatorsStatus();
}

void ControlWidget::on_12_1v_clicked(bool checked)
{
    MsgSetTurnOn_12v_1 msg(_evapInterface);
    msg.isOn = !checked;
    if (!_ms->postAsync(&msg)) {
        static_cast<LedLabel*>(sender())->setState(LedLabel::Disabled);
        return;
    }
    updateControlIndicatorsStatus();
}

void ControlWidget::on_12_2v_clicked(bool checked)
{
    MsgSetTurnOn_12v_2 msg(_evapInterface);
    msg.isOn = !checked;
    if (!_ms->postAsync(&msg)) {
        static_cast<LedLabel*>(sender())->setState(LedLabel::Disabled);
        return;
    }
    updateControlIndicatorsStatus();
}

void ControlWidget::sendTurnOnIndicators(bool checked)
{
    LedLabel* ll = qobject_cast<LedLabel*>(sender());
    MsgTurnOnIndicators msg(_evapInterface);
    msg.power = ll == ui->label_Indicators_Power? !checked: ui->label_Indicators_Power->isOn();
    msg.module = ll == ui->label_Indicators_Module? !checked: ui->label_Indicators_Module->isOn();
    msg.isReady = ll == ui->label_Indicators_isReady? !checked: ui->label_Indicators_isReady->isOn();
    msg.ventilation = ll == ui->label_Indicators_Ventilation? !checked: ui->label_Indicators_Ventilation->isOn();

    if(!_ms->postAsync(&msg)) {
        ui->label_Indicators_Power->setState(LedLabel::Disabled);
        ui->label_Indicators_Module->setState(LedLabel::Disabled);
        ui->label_Indicators_isReady->setState(LedLabel::Disabled);
        ui->label_Indicators_Ventilation->setState(LedLabel::Disabled);
        return;
    }
    updateControlIndicatorsStatus();
}

void ControlWidget::on_pushButtonReset_clicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    MsgMicrocontrollerReboot msg(_evapInterface);
    _ms->postAsync(&msg);
    UtilsForGui::setWidgetStyleHlt(btn,  UtilsForGui::StyleWidgetOn);
    btn->setToolTip(msg.errMsg());
    emit resetMC();
}

void ControlWidget::on_pushButtonSelfControl_clicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    MsgSelfControl msg(_evapInterface);
    if (!_ms->postAsync(&msg)) {
        UtilsForGui::setWidgetStyleHlt(btn,  UtilsForGui::StyleWidgetOff);
        btn->setToolTip(msg.errMsg());
    } else {
        UtilsForGui::setWidgetStyleHlt(btn,  UtilsForGui::StyleWidgetOn);
    }
}

void ControlWidget::on_pushButtonCalibrationADC_clicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    MsgCalibrationADC msg(_evapInterface);
    msg.adc_5V = quint32(ui->cal_5V_spinBox->value());
    msg.adc_12V_Aux = quint32(ui->cal_12V_AUX_spinBox->value());
    msg.adc_3V3_AUX = quint32(ui->cal_3V3_AUX_spinBox->value());

    if (!_ms->postAsync(&msg)) {
        UtilsForGui::setWidgetStyleHlt(btn,  UtilsForGui::StyleWidgetOff);
        btn->setToolTip(msg.errMsg());
    }
    else {
        UtilsForGui::setWidgetStyleHlt(btn,  UtilsForGui::StyleWidgetOn);
    }
}

void ControlWidget::initUI()
{
    ui->spinBoxLowerFans->setStyleSheet("");
    ui->spinBoxLowerFans->setToolTip(tr("<html><head/><body><p><span style=\"font-weight:600;\">"
                                         "устанавливает скорость вентиляторов нижней полки"
                                         "</span></p></body></html>"));
    ui->spinBoxHigherFans->setStyleSheet("");
    ui->spinBoxHigherFans->setToolTip(tr("<html><head/><body><p><span style=\"font-weight:600;\">"
                                     "устанавливает скорость вентиляторов верхней полки"
                                     "</span></p></body></html>"));
    ui->pushButtonReset->setToolTip(tr("<html><head/><body><p><span style=\" font-weight:600;\">"
                                    "Выполняет перезагрузку контроллера, эквивалентную аппаратному сигналу RESET"
                                    "</span></p></body></html>"));
    ui->pushButtonSelfControl->setToolTip(tr("<html><head/><body><p><span style=\"font-size:10pt; font-weight:600;\">"
                                             "Выполняет операцию, эквивалентную первичному запуску ИВЭП с контролем состояния."
                                             "</span></p><p>Поскольку первичный запуск пытается включить источник питания, но "
                                             "ничего не отключает, эта команда не влияет на состояние включенных узлов, но "
                                             "может включить выключенные узлы.</p></body></html>"));
    ui->pushButtonCalibrationADC->setToolTip(tr("<html><head/><body><p><span style=\"font-weight:600; color:#000000;\">"
                                                "Cохранить замеры кодов АЦП во флэш-памяти контроллера."
                                                "</span></p></body></html>"));
}

void ControlWidget::initResetCmd()
{
    UtilsForGui::setWidgetStyleHlt(ui->pushButtonReset, UtilsForGui::StyleWidgetClear);
}

void ControlWidget::on_spinBoxLowerFans_valueChanged(int value)
{
    QSpinBox* sb = qobject_cast<QSpinBox*>(sender());
    MsgSetFanSpeed msg(_evapInterface);
    msg.setFanSpeed(1, quint16(value));
    if (!_ms->postAsync(&msg)) {
        UtilsForGui::setWidgetStyleHlt(sb,  UtilsForGui::StyleWidgetOff);
        ui->labelLowerAuto->setText("");
        return;
    }
    updateFanSpeed();
}

void ControlWidget::on_spinBoxHigherFans_valueChanged(int value)
{
    QSpinBox* sb = qobject_cast<QSpinBox*>(sender());
    MsgSetFanSpeed msg(_evapInterface);
    msg.setFanSpeed(2, quint16(value));
    if (!_ms->postAsync(&msg)) {
        UtilsForGui::setWidgetStyleHlt(sb,  UtilsForGui::StyleWidgetOff);
        return;
    }
    updateFanSpeed();
}

void ControlWidget::init(EvapInterface *evapInterface, EvapMessageService *ms)
{
    _evapInterface = evapInterface;
    _ms = ms;
}

void ControlWidget::initTableFans()
{
    foreach (LedLabel* ll, ui->groupBox_FanSpeedMonitor->findChildren<LedLabel*>()) {
        ll->setStylesheetOn("QLabel {border: 2px solid black; border-radius: 15px; background: lightgreen;"
                            " min-height: 28px;min-width: 28px;}");
        ll->setStylesheetOff("QLabel {border: 2px solid black; border-radius: 15px; background: red;"
                            " min-height: 30px;min-width: 30px;}");
    };
}

void ControlWidget::initConnect()
{
    bool connected = false;

    connected = connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    Q_ASSERT(connected);

    connected = connect(ui->label_Indicators_Power, SIGNAL(clicked(bool)), this,
                        SLOT(sendTurnOnIndicators(bool)));
    Q_ASSERT(connected);
    connected = connect(ui->label_Indicators_Module, SIGNAL(clicked(bool)), this,
                        SLOT(sendTurnOnIndicators(bool)));
    Q_ASSERT(connected);
    connected = connect(ui->label_Indicators_isReady, SIGNAL(clicked(bool)), this,
                        SLOT(sendTurnOnIndicators(bool)));
    Q_ASSERT(connected);
    connected = connect(ui->label_Indicators_Ventilation, SIGNAL(clicked(bool)), this,
                        SLOT(sendTurnOnIndicators(bool)));
    Q_ASSERT(connected);
    connected = connect(ui->label_Indicators_SP, SIGNAL(clicked(bool)), this,
                        SLOT(on_PS_clicked(bool)));
    Q_ASSERT(connected);
    connected = connect(ui->label_Indicators_24V, SIGNAL(clicked(bool)), this,
                        SLOT(on_24v_clicked(bool)));
    Q_ASSERT(connected);
    connected = connect(ui->label_Indicators_12v1, SIGNAL(clicked(bool)), this,
                        SLOT(on_12_1v_clicked(bool)));
    Q_ASSERT(connected);
    connected = connect(ui->label_Indicators_12v2, SIGNAL(clicked(bool)), this,
                        SLOT(on_12_2v_clicked(bool)));
    Q_ASSERT(connected);
}
