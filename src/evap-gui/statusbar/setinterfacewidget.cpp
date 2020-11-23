#include "setinterfacewidget.h"
#include "ui_setinterfacewidget.h"
#include "setipconfigevapdialog.h"
#include "../gui-lib/utilsforgui.h"

SetInterfaceWidget::SetInterfaceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetInterfaceWidget)
{
    ui->setupUi(this);
    devActionList << tr("Подкл.") << tr("Откл.") ;
}

SetInterfaceWidget::~SetInterfaceWidget()
{
    delete ui;
}

void SetInterfaceWidget::showStatus()
{
    EvapInterface *ef = qobject_cast<EvapInterface*>(sender());
    ui->pushButtonOpenDevice->setText(devActionList[int(curEvapInterface->isOpen())]);
    if (ef->isOpen())
    {
        ui->pushButtonOpenDevice->setChecked(true);
        emit curEvapInterfaceChanged(curEvapInterface);
    }
    else
        ui->pushButtonOpenDevice->setChecked(false);
}

EvapInterface *SetInterfaceWidget::openDevice()
{
    qDebug() << "-----openDevice()---------";
    ui->pushButtonOpenDevice->setChecked(false);
    if (!curEvapInterface->isOpen()) {
        _ms->start();
        curEvapInterface->open();
    }

    ui->pushButtonOpenDevice->setText(devActionList[int(curEvapInterface->isOpen())]);
    return curEvapInterface;
}

void SetInterfaceWidget::closeDevice()
{
    qDebug() << "-----closeDevice()---------";
    ui->pushButtonOpenDevice->setChecked(false);
    curEvapInterface->close();
    _ms->stop();
    curEvapInterface->close();
}

void SetInterfaceWidget::evapConfigureChanged()
{
    EvapInterface* ev = qobject_cast<EvapInterface*>(sender());
    Q_ASSERT(ev != NULL);

    ui->comboBoxInterface->setItemText(evapInterfaceList.indexOf(ev), ev->toString());
}

void SetInterfaceWidget::on_device_disconnect()
{
    showStatus();
}

void SetInterfaceWidget::on_device_connect()
{
    showStatus();
}

void SetInterfaceWidget::init(QList<EvapInterface *> evapInterfaceList,
                              EvapMessageService *ms)
{
    _ms = ms;
    this->evapInterfaceList = evapInterfaceList;
    curEvapInterface = this->evapInterfaceList.first();
    ui->comboBoxInterface->clear();
    bool connected;

    foreach (EvapInterface* ef, this->evapInterfaceList) {
        ui->comboBoxInterface->addItem(ef->toString());
        connected = connect(ef, SIGNAL(configureChanged()), this, SLOT(evapConfigureChanged()));
        Q_ASSERT(connected);
        connected = connect(ef, SIGNAL(closed()), this, SLOT(on_device_disconnect()));
        Q_ASSERT(connected);
        connected = connect(ef, SIGNAL(opened()), this, SLOT(on_device_connect()));
        Q_ASSERT(connected);
    }

    connected = connect(ui->comboBoxInterface, SIGNAL(currentIndexChanged(int)),
                        this, SLOT(currentEvapInterfaceChanged(int)));
    Q_ASSERT(connected);
}

void SetInterfaceWidget::on_toolButtonSetupInterface_clicked()
{
    EvapInterface* evapIface = evapInterfaceList.at(ui->comboBoxInterface->currentIndex());
    if (evapIface->typeInterface() == EvapInterface::TCP) {
        SetIpConfigEvapDialog *dialog = new SetIpConfigEvapDialog(qobject_cast<EvapTCP*>(evapIface)) ;
        dialog->show();
    }
/// TODO
    if (evapIface->typeInterface() == EvapInterface::SERIALPORT) {
//        dialog->show();
    }
}

void SetInterfaceWidget::currentEvapInterfaceChanged(int index)
{
    EvapInterface* ef = evapInterfaceList.at(index);
    ef->open();
}

void SetInterfaceWidget::on_pushButtonOpenDevice_clicked()
{
    if (qobject_cast<QPushButton*>(sender())->isChecked())
        openDevice();
    else
        closeDevice();
}
