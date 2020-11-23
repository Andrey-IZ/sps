#include "monitorwidget.h"
#include "ui_monitorwidget.h"
#include <QCheckBox>
#include <QDebug>
#include "../gui-lib/component/ledlabel.h"
#include "../gui-lib/utilsforgui.h"
#include "../evap-protocol/msggetbcmstate.h"
#include "../evap-protocol/msggetkpowercontrol.h"
#include "../evap-protocol/msgreadblockaddress.h"
#include "../evap-protocol/msgreadaccidentregister.h"
#include "../evap-protocol/msggetzvscontrol.h"
#include "../evap-protocol/msggetpstemp.h"
#include "../evap-protocol/msggetspscontrol.h"


MonitorWidget::MonitorWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MonitorWidget), timeoutUpdate(500),
    timeoutStop(true)
{
    ui->setupUi(this);

    initTableBCM();
    initTempPS();
    initConnect();
}

void MonitorWidget::init(EvapInterface *evapInterface, EvapMessageService *ms)
{
   _evapInterface = evapInterface;
   _ms = ms;
}

MonitorWidget::~MonitorWidget()
{
    timeoutStop = true;
    delete ui;
}

void MonitorWidget::updateControls()
{
    updateBCM();
    updateMPFC();
    updateSPS();
    updateBlockAddress();
    updateDamageInfo();
    updateZVS();
    updateTempPS();
}

void MonitorWidget::startMonitor(int timeout)
{
    timeoutStop = false;
    timeoutUpdate = timeout;
    timer.start(0);
}

void MonitorWidget::stopMonitor()
{
    timeoutStop = true;
}

void MonitorWidget::timeout()
{
    Q_ASSERT(_evapInterface != NULL);
    timer.stop();
    elTimer.restart();

    updateControls();

    int time = timeoutUpdate - int(elTimer.elapsed());
    qDebug() << "time=" << time;
    if (!timeoutStop)
        timer.start(time > 0? time: 0);
}

void MonitorWidget::updateBCM()
{
    MsgGetBCMstate msg(_evapInterface);
    if(!_ms->getAsync(&msg)) {
        ui->tableWidgetBCM->setEnabled(false);
        return;
    }
    ui->tableWidgetBCM->setEnabled(true);

    for (int i = 0; i < msg.bcmList.count(); ++i) {
        ui->tableWidgetBCM->setCellValue(i, 0, QString(" %1 ").arg(msg.bcmList.at(i).Vin));
        ui->tableWidgetBCM->setCellValue(i, 1, QString(" %1 ").arg(msg.bcmList.at(i).Vout));
        ui->tableWidgetBCM->setCellValue(i, 2, QString(" %1 ").arg(msg.bcmList.at(i).Iin));
        ui->tableWidgetBCM->setCellValue(i, 3, QString(" %1 ").arg(msg.bcmList.at(i).Iout));
        ui->tableWidgetBCM->setCellValue(i, 4, QString(" %1 ").arg(msg.bcmList.at(i).T));
    }
}

void MonitorWidget::updateMPFC()
{

    MsgGetMPFC msg(_evapInterface);
    if(!_ms->getAsync(&msg)) {
        UtilsForGui::setEnabledAllWidgets(ui->groupBox_MPFC_ACGO, false);
        return;
    }
    UtilsForGui::setEnabledAllWidgets(ui->groupBox_MPFC_ACGO, true);

    QList<LedLabel*> ledLabelList2 = ui->groupBox_MPFC_ACGO->findChildren<LedLabel*>();

    for (int i = 0; i < ledLabelList2.count(); ++i) {
        bool value = msg.mpfc.at(ledLabelList2.at(i)->text().toInt()-1);
        ledLabelList2[i]->setLedOn(value);
    }
}

void MonitorWidget::updateSPS()
{
    MsgGetSPSControl msg(_evapInterface);
    if (!_ms->getAsync(&msg)) {
        UtilsForGui::setEnabledAllWidgets(ui->groupBox_SPS, false);
        return;
    }
    UtilsForGui::setEnabledAllWidgets(ui->groupBox_SPS, true);
    ui->label_PG_5V->setLedOn(msg.PG_5V);
    ui->label_24V->setLedOn(msg.sps_24V);
    ui->label_5V->setLedOn(msg.sps_5V);
    ui->label_12VAux->setLedOn(msg.sps_12VAux);
    ui->label_3V3AUX->setLedOn(msg.sps_3V3AUX);
    ui->label_minus_12V->setLedOn(msg.sps_minus_12V);
}

void MonitorWidget::updateBlockAddress()
{
    MsgReadBlockAddress msg(_evapInterface);
    if(!_ms->getAsync(&msg)) {
        UtilsForGui::setEnabledAllWidgets(ui->groupBox_BlockAddress, false);
        return;
    }
    UtilsForGui::setEnabledAllWidgets(ui->groupBox_BlockAddress, true);
    ui->label_BlockAddress_OH_PI4->setLedOn(msg.blockAddress >> 0 & 1);
    ui->label_BlockAddress_OH_PI5->setLedOn(msg.blockAddress >> 1 & 1);
    ui->label_BlockAddress_OH_PI6->setLedOn(msg.blockAddress >> 2 & 1);
}

void MonitorWidget::updateDamageInfo()
{
    MsgReadAccidentRegister msg(_evapInterface);
    if (!_ms->getAsync(&msg)) {
        emit nothingHappened();
        return;
    }

    if (msg.somethingWentWrong) {
        emit showDamagedInfo(msg.damageMsg());
    }
    else {
        emit showDamagedInfo(QStringList());
    }
}

void MonitorWidget::updateZVS()
{
    MsgGetZVSControl msg(_evapInterface);
    if (!_ms->getAsync(&msg)){
        UtilsForGui::setEnabledAllWidgets(ui->groupBox_ZVS, false);
        return;
    }
    UtilsForGui::setEnabledAllWidgets(ui->groupBox_ZVS, true);
    QList<LedLabel*> zvsPinList = ui->groupBox_ZVS->findChildren<LedLabel*>();
    QRegExp reIn("label_zvs_in_(\\d+)");
    QRegExp reOut("label_zvs_out_(\\d+)");

    foreach (LedLabel *ll, zvsPinList) {
        if (reIn.indexIn(ll->objectName(), 0) != -1) {
            int num = reIn.cap(1).toInt() - 1;
            ll->setLedOn(msg.inZVS.at(num));
        }
        if (reOut.indexIn(ll->objectName(), 0) != -1) {
            int num = reOut.cap(1).toInt() - 1;
            ll->setLedOn(msg.outZVS.at(num));
        }
    }
}

void MonitorWidget::updateTempPS()
{
    MsgGetPStemp msg(_evapInterface);
    if (!_ms->getAsync(&msg)) {
        UtilsForGui::setEnabledAllWidgets(ui->groupBox_Temp, false);
        return;
    }
    UtilsForGui::setEnabledAllWidgets(ui->groupBox_Temp, true);
    ui->label_temp_OC->setText(QString::number(msg.temp));
    ui->label_temp_OC->setLedOn(msg.isTempOk());
    ui->label_temp_D14->setText(QString::number(msg.IMS_D14));
    ui->label_temp_D14->setLedOn(msg.isD14Ok());
    ui->label_temp_D15->setText(QString::number(msg.IMS_D15));
    ui->label_temp_D15->setLedOn(msg.isD15Ok());
}

void MonitorWidget::initTempPS()
{
    LedLabel* temp = ui->groupBox_Temp->findChild<LedLabel*>("label_temp_OC");
    temp->setTitle("OC");
//    temp->initLayout(false);
    temp->setAlignment(Qt::AlignCenter);
    temp->setAlignmentTitle(Qt::AlignCenter);
    LedLabel* tempD14 = ui->groupBox_Temp->findChild<LedLabel*>("label_temp_D14");
    tempD14->setTitle("D14");
//    tempD14->initLayout(false);
    tempD14->setAlignment(Qt::AlignCenter);
    tempD14->setAlignmentTitle(Qt::AlignCenter);
    LedLabel* tempD15 = ui->groupBox_Temp->findChild<LedLabel*>("label_temp_D15");
    tempD15->setTitle("D15");
//    tempD15->initLayout(false);
    tempD15->setAlignment(Qt::AlignCenter);
    tempD15->setAlignmentTitle(Qt::AlignCenter);
}

void MonitorWidget::initTableBCM()
{
    QList<QList<QString> >listLabels;
    for (int i = 0; i < 4; ++i) {
            listLabels.append(QList<QString>() << "0" << "0" << "0" << "0" << "0");
    }
    ui->tableWidgetBCM->initTableStateView(QStringList() << "1" << "2" << "3" << "4",
                                           QStringList() << "Vin" << "Vout" << "Iin" << "Iout" << "T",
                                           listLabels);
    ui->tableWidgetBCM->setAllCellStatus(LedLabel::On);
    ui->tableWidgetBCM->setAllCellStylesheet(" ", " ");
}

void MonitorWidget::initConnect()
{
    bool connected = false;
    connected = connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    Q_ASSERT(connected);
}
