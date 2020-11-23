#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QMainWindow>
#include <QPlainTextEdit>
#include "../evap/evapmessageservice.h"
#include "../gui-lib/notification/popup.h"
#include "../evap-gui/statusbar/idinfowidget.h"
#include "../evap-gui/statusbar/setinterfacewidget.h"
#include "setup/setidwidget.h"
#include "setup/setupipconfigwidget.h"

#define TIMEOUT_CONTROL 500

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    EvapMessageService *ms;

signals:
    void popupTextAdded(QString text);

private slots:
    void on_processEvent();
    void showPopUp(QString text);
    void queueCommandsChanged(const QQueue<Command*> queue);
    void commandSent(const Command* cmd);
    void on_ErrorMessagesGot(QString msgText);
    void on_device_disconnect();
    void on_device_connect();
    void curEvapInterfaceChanged(EvapInterface* evapInterface);
    void on_actionDebug_triggered();
    void tabExpanded(bool isExpanded);

    void on_actionChangeID_triggered();
    void on_action_ChangeIP_triggered();

    void showDamagedInfo(QStringList messgesList);
    void clearAccidentList();

    void on_actionAbout_triggered();
    void tryToReconnect();
    void showResetMCStatus();
    void startWaiting();

private:
    Ui::MainWindow *ui;
    EvapInterface* curEvapInterface;
    QList<EvapInterface*> evapInterfaceList;
    PopUp *_popUpQueueMsgNotifier;
    QStringList _commandsList;
    IdInfoWidget *idInfoWidget;
    SetInterfaceWidget *setInterfaceWidget;
    SetIDWidget *widgetID;
    SetupIPConfigWidget *widgetIPconfig;
    QTimer timerReset;
    QTimer timerReconnect;
    int resetTime;

    const int tabSize;

    void initUI();
    void initStatusBar();
    void initService();
    void initConnect();
    void initControlWidget();

    void squeezeSize();
    
protected:
    virtual void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
