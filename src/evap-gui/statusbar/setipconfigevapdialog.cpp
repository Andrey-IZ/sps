#include "setipconfigevapdialog.h"
#include "ui_setipconfigevapdialog.h"
#include "settings.h"

SetIpConfigEvapDialog::SetIpConfigEvapDialog(EvapTCP *evapTcp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetIpConfigEvapDialog), evapTcp(evapTcp)
{
    ui->setupUi(this);
    QSettings s (ORGANIZATION_NAME, APPLICATION_NAME);
    ui->IpaddressLineEdit->setText(s.value(SETTINGS_TCP_IP, "169.254.4.143").toString());
    ui->portSpinBox->setValue(s.value(SETTINGS_TCP_PORT, 8080).toInt());
}

SetIpConfigEvapDialog::~SetIpConfigEvapDialog()
{
    delete ui;
}

void SetIpConfigEvapDialog::on_pushButtonSend_clicked()
{
    Q_ASSERT(evapTcp != NULL);

    QSettings settings (ORGANIZATION_NAME, APPLICATION_NAME);
    settings.setValue(SETTINGS_TCP_IP, ui->IpaddressLineEdit->text());
    settings.setValue(SETTINGS_TCP_PORT, ui->portSpinBox->value());
    settings.sync();

    evapTcp->close();
    evapTcp->setupTCP(ui->IpaddressLineEdit->text(), ui->portSpinBox->value());
    evapTcp->open();

    close();
}
