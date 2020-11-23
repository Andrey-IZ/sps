#include "idinfowidget.h"
#include "ui_idinfowidget.h"
#include "../evap-protocol/msggetidinfo.h"
#include "../gui-lib/utilsforgui.h"

IdInfoWidget::IdInfoWidget(EvapInterface *evapInterface, EvapMessageService *ms,
                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IdInfoWidget), _evapInterface(evapInterface), _ms(ms)
{
    ui->setupUi(this);
}

IdInfoWidget::~IdInfoWidget()
{
    delete ui;
}

void IdInfoWidget::updateIdInfo()
{
    if(!_evapInterface->isOpen())
        return;

    MsgGetIdInfo msg(_evapInterface);
    if(_ms->getAsync(&msg)) {
        ui->labelTitile->setText(QString("%1v (%2)").arg(msg.versionToStr()).
                                 arg(msg.releaseDate.toString()));
        this->setToolTip(msg.toInfoList().join("\n"));
    } else {
        this->setToolTip(msg.errMsg());
    }
}


void IdInfoWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    emit doubleClicked();
}

void IdInfoWidget::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
}
