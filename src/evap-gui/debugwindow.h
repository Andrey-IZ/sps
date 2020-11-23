#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QMainWindow>
#include "../evap-protocol/interfaces/evapinterface.h"
#include "../evap/evapmessageservice.h"

namespace Ui {
class DebugWindow;
}

class DebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DebugWindow(EvapInterface *evapInterface, EvapMessageService *ms,
                         QWidget *parent = 0);
    ~DebugWindow();

private slots:
    void on_msg_sent(quint32 code, QByteArray data);
    void on_msg_recv(quint32 code, QByteArray data);
    void on_error_caught(const QString message);

    void on_pushButtonPost_clicked();
    void on_pushButtonGet_clicked();

private:
    Ui::DebugWindow *ui;
    EvapInterface *_evapInterface;
    EvapMessageService *_ms;

    int row;
    void initTable();
    void addRecord(EvapInterface *interface, quint32 code, QByteArray &data,
                   QString direction = "->");
};

#endif // DEBUGWINDOW_H
