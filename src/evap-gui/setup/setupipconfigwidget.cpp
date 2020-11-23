#include "setupipconfigwidget.h"
#include "ui_setupipconfigwidget.h"
#include "../evap-protocol/msgsetip_config.h"
#include "../gui-lib/utilsforgui.h"
#include "settings.h"

SetupIPConfigWidget::SetupIPConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupIPConfigWidget)
{
    ui->setupUi(this);
    toolTip = tr("смена базового IP-адреса, маски подсети и порта");
    QSettings s (ORGANIZATION_NAME, APPLICATION_NAME);
    ui->IpaddressLineEdit->setText(s.value(SETTINGS_TCP_IP, "192.168.1.1").toString());
    ui->portSpinBox->setValue(s.value(SETTINGS_TCP_PORT, 8080).toInt());
}

SetupIPConfigWidget::~SetupIPConfigWidget()
{
    delete ui;
}

void SetupIPConfigWidget::init(EvapInterface *evapInterface, EvapMessageService *ms)
{
    _evapInterface = evapInterface;
    _ms = ms;
}

void SetupIPConfigWidget::on_pushButtonSend_clicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());

    MsgSetIP_Config msg(_evapInterface);
    msg.set_ip_address(ui->IpaddressLineEdit->text());
    msg.set_subnet_mask(ui->subnetMaskLineEdit->text());
    msg.port = quint32(ui->portSpinBox->value());
    if(_ms->postAsync(&msg)) {
        UtilsForGui::setWidgetStyleHlt(btn, UtilsForGui::StyleWidgetOn);
        btn->setToolTip(toolTip);
        emit finished();
    } else {
        UtilsForGui::setWidgetStyleHlt(btn, UtilsForGui::StyleWidgetOff);
        btn->setToolTip(msg.errMsg());
    }
}
