#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>
#include "../evap-protocol/interfaces/evapinterface.h"
#include "../evap/evapmessageservice.h"

namespace Ui {
class MonitorWidget;
}

class MonitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWidget(QWidget *parent = 0);
    ~MonitorWidget();

public slots:
    void init(EvapInterface *evapInterface, EvapMessageService *ms);
    void updateControls();
    void startMonitor(int timeout);
    void stopMonitor();

signals:
    void showDamagedInfo(QStringList list);
    void nothingHappened();

private slots:
    void timeout();

private:
    Ui::MonitorWidget *ui;
    EvapInterface *_evapInterface;
    EvapMessageService *_ms;
    QTimer timer;
    QElapsedTimer elTimer;
    int timeoutUpdate;
    bool timeoutStop;

    void updateBCM();
    void updateMPFC();
    void updateSPS();
    void updateBlockAddress();
    void updateDamageInfo();
    void updateZVS();
    void updateTempPS();

    void initTableBCM();
    void initConnect();
    void initTempPS();
};

#endif // MONITORWIDGET_H
