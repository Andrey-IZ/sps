#ifndef SETUPIPCONFIGWIDGET_H
#define SETUPIPCONFIGWIDGET_H

#include <QWidget>
#include "../evap-protocol/interfaces/evapinterface.h"
#include "../evap/evapmessageservice.h"

namespace Ui {
class SetupIPConfigWidget;
}

class SetupIPConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetupIPConfigWidget(QWidget *parent = 0);
    ~SetupIPConfigWidget();

    void init(EvapInterface *evapInterface, EvapMessageService *ms);

signals:
    void finished();

private slots:
    void on_pushButtonSend_clicked();

private:
    Ui::SetupIPConfigWidget *ui;
    EvapInterface *_evapInterface;
    EvapMessageService *_ms;
    QString toolTip;
};

#endif // SETUPIPCONFIGWIDGET_H
