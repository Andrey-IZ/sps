#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "../evap-protocol/msggetidinfo.h"

AboutDialog::AboutDialog(EvapInterface *evapInterface, EvapMessageService *ms,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    _evapInterface = evapInterface;
    _ms = ms;

    init();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::enableUI(bool isEnabled)
{
    ui->companyLineEdit->setEnabled(isEnabled);
    ui->decimalNumberLineEdit->setEnabled(isEnabled);
    ui->sreiesLineEdit->setEnabled(isEnabled);
    ui->dateReleaseLineEdit->setEnabled(isEnabled);
    ui->versionLineEdit->setEnabled(isEnabled);
    ui->crcLineEdit->setEnabled(isEnabled);
}

void AboutDialog::init()
{
    MsgGetIdInfo msg(_evapInterface);
    if (_ms->getAsync(&msg)) {
        enableUI(true);
        ui->companyLineEdit->setText(QString("%1").arg(msg.idToStr()));
        ui->decimalNumberLineEdit->setText(QString("%1").arg(msg.moduleNumber));
        ui->sreiesLineEdit->setText(QString("%1").arg(msg.serialNumber));
        ui->dateReleaseLineEdit->setText(msg.releaseDate.toString());
        ui->versionLineEdit->setText(msg.versionToStr());
        ui->crcLineEdit->setText(QString::number(msg.crc, 16));
    } else {
        enableUI(false);
    }
}
