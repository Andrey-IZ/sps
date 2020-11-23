#ifndef SETINTERFACEWIDGET_H
#define SETINTERFACEWIDGET_H

#include <QWidget>
#include "../evap-protocol/interfaces/evapinterface.h"
#include "../evap/evapmessageservice.h"


namespace Ui {
class SetInterfaceWidget;
}

class SetInterfaceWidget : public QWidget
{
    Q_OBJECT

public:

    explicit SetInterfaceWidget(QWidget *parent = 0);
    ~SetInterfaceWidget();

    void init(QList<EvapInterface*>  evapInterfaceList, EvapMessageService *ms);

signals:
    void curEvapInterfaceChanged(EvapInterface* evapInterface);

public slots:
    EvapInterface* openDevice();
    void closeDevice();
    void evapConfigureChanged();

private slots:
    void on_device_disconnect();
    void on_device_connect();
    void on_toolButtonSetupInterface_clicked();
    void currentEvapInterfaceChanged(int index);
    void on_pushButtonOpenDevice_clicked();

private:
    Ui::SetInterfaceWidget *ui;
    QList<EvapInterface*>  evapInterfaceList;
    EvapInterface* curEvapInterface;
    EvapMessageService *_ms;
    QStringList devActionList;

    void showStatus();
};

#endif // SETINTERFACEWIDGET_H
