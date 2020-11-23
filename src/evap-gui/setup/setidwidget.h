#ifndef SETIDWIDGET_H
#define SETIDWIDGET_H

#include <QWidget>
#include "../evap-protocol/interfaces/evapinterface.h"
#include "../evap/evapmessageservice.h"

namespace Ui {
class SetIDWidget;
}

class SetIDWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetIDWidget(QWidget *parent = 0);
    ~SetIDWidget();

    void init(EvapInterface *evapInterface, EvapMessageService *ms);

signals:
    void finished();

private slots:
    void on_pushButtonSend_clicked();

private:
    Ui::SetIDWidget *ui;
    EvapInterface *_evapInterface;
    EvapMessageService *_ms;
    QString toolTip;
};

#endif // SETIDWIDGET_H
