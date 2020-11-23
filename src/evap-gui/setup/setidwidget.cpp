#include "setidwidget.h"
#include "ui_setidwidget.h"
#include "../evap-protocol/msgsetid.h"
#include "../gui-lib/utilsforgui.h"

SetIDWidget::SetIDWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetIDWidget)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    toolTip = tr("занесение идентификационных данных");
}

SetIDWidget::~SetIDWidget()
{
    delete ui;
}

void SetIDWidget::init(EvapInterface *evapInterface, EvapMessageService *ms)
{
    _evapInterface = evapInterface;
    _ms = ms;
}

void SetIDWidget::on_pushButtonSend_clicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    MsgSetId msg(_evapInterface);
    msg.serialNumber = quint32(ui->spinBoxRelease->value());
    msg.releaseDate = ui->dateEdit->date();
    if(_ms->postAsync(&msg)) {
        UtilsForGui::setWidgetStyleHlt(ui->pushButtonSend, UtilsForGui::StyleWidgetOn);
        btn->setToolTip(toolTip);
        emit finished();
    } else {
        UtilsForGui::setWidgetStyleHlt(ui->pushButtonSend, UtilsForGui::StyleWidgetOff);
        btn->setToolTip(msg.errMsg());
    }
}
