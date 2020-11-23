#include "debugwindow.h"
#include "ui_debugwindow.h"
#include "../gui-lib/utilsforgui.h"

DebugWindow::DebugWindow(EvapInterface *evapInterface, EvapMessageService *ms, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DebugWindow), _evapInterface(evapInterface), _ms(ms), row(0)
{
    ui->setupUi(this);

    bool connected;
    connected = connect(evapInterface, SIGNAL(msgSent(quint32, QByteArray)), this,
            SLOT(on_msg_sent(quint32, QByteArray)));
    Q_ASSERT(connected);
    connected = connect(evapInterface, SIGNAL(msgRecv(quint32, QByteArray)), this,
            SLOT(on_msg_recv(quint32, QByteArray)));
    Q_ASSERT(connected);
    connected = connect(evapInterface, SIGNAL(error(const QString )), this,
            SLOT(on_error_caught(const QString )));
    Q_ASSERT(connected);
}

DebugWindow::~DebugWindow()
{
    delete ui;
}

void DebugWindow::initTable()
{
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

void DebugWindow::addRecord(EvapInterface *interface, quint32 code, QByteArray &data, QString direction)
{
    if (interface == NULL) return;

    QString typeStr;

    switch (interface->typeInterface())
    {
        case EvapInterface::TCP:
            typeStr = "TCP";
            break;
        case EvapInterface::SERIALPORT:
            typeStr = "SERIALPORT";
            break;
    }

    int column = 0;
    ui->tableWidget->setRowCount(row + 1);
    ui->tableWidget->setItem(row, column++, new QTableWidgetItem(direction));
    ui->tableWidget->setItem(row, column++, new QTableWidgetItem(typeStr));
    ui->tableWidget->setItem(row, column++, new QTableWidgetItem(QString::number(code)));
    ui->tableWidget->setItem(row, column++, new QTableWidgetItem(UtilsForGui::ByteArrayToHexString(data)));
    ui->tableWidget->selectRow(row);
    row++;
}

void DebugWindow::on_msg_sent(quint32 code, QByteArray data)
{
    addRecord(qobject_cast<EvapInterface*>(sender()), code, data, "(s) ->");
}

void DebugWindow::on_msg_recv(quint32 code, QByteArray data)
{
    addRecord(qobject_cast<EvapInterface*>(sender()), code, data, "-> (r)");
}

void DebugWindow::on_error_caught(const QString message)
{
    QByteArray ba;

    int column = 0;
    ui->tableWidget->setRowCount(row + 1);
    ui->tableWidget->setItem(row, column++, new QTableWidgetItem("Error"));
    ui->tableWidget->setItem(row, column++, new QTableWidgetItem(""));
    ui->tableWidget->setItem(row, column++, new QTableWidgetItem(QString::number(-1)));
    ui->tableWidget->setItem(row, column++, new QTableWidgetItem(message));
    row++;
}

void DebugWindow::on_pushButtonPost_clicked()
{
    _ms->postAsync(ui->codeSpinBox->value(),
                   UtilsForGui::HexStringToByteArray(ui->dataLineEdit->text()),
                   _evapInterface, "debug msg");
}

void DebugWindow::on_pushButtonGet_clicked()
{
    QByteArray data(ui->codeGetDataLengthSpinBox->value(), '0');
    _ms->getAsync(ui->codeGetSpinBox->value(), data, _evapInterface, "debug msg");
}
