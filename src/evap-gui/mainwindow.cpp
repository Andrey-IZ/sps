#include "debugwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"

#include "setup/setupdialog.h"
#include "aboutdialog.h"
#include "../gui-lib/utilsforgui.h"
#include "../evap-protocol/interfaces/evaptcp.h"
#include "../evap-protocol/interfaces/evapserial.h"

#include "../evap-protocol/msggetidinfo.h"
#include "../evap-protocol/msgmicrocontrollerreboot.h"
#include "../evap-protocol/msgreadblockaddress.h"
#include "../evap-protocol/msgreadindicatorstatus.h"
#include "../evap-protocol/msgturnonindicators.h"
#include "../evap-protocol/msgselfcontrol.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), tabSize(0)
{
    ui->setupUi(this);

    ms = new EvapMessageService(this);

    initService();
    initUI();
    initStatusBar();
    initControlWidget();
    initConnect();
}

MainWindow::~MainWindow()
{
    ms->stop();
    delete widgetIPconfig;
    delete widgetID;
    delete ui;
}

void MainWindow::initService()
{
    bool connected;

    connected = connect(ms, SIGNAL(eventLoop()), this, SLOT(on_processEvent()),
                        Qt::DirectConnection);
    Q_ASSERT(connected);

    EvapTCP *evapTcp= new EvapTCP(this);
    /// TODO
//    EvapSerial * evapSerial = new EvapSerial(this);

    ms->start();
    connected = connect(ms->getWorker(), SIGNAL(queueCommandsChanged(const QQueue<Command*>)),
                        this, SLOT(queueCommandsChanged(const QQueue<Command*>)),
                        Qt::DirectConnection);
    Q_ASSERT(connected);

    connected = connect(ms->getWorker(), SIGNAL(commandSent(const Command*)), this,
                        SLOT(commandSent(const Command*)), Qt::DirectConnection);
    Q_ASSERT(connected);


    QSettings settings (ORGANIZATION_NAME, APPLICATION_NAME);
    evapTcp->setupTCP(settings.value(SETTINGS_TCP_IP, "192.168.1.1").toString(),
                      settings.value(SETTINGS_TCP_PORT, 8080).toInt());

    connected = connect(evapTcp, SIGNAL(error(QString)), this,
                        SLOT(on_ErrorMessagesGot(QString)));
    Q_ASSERT(connected);
    connected = connect(evapTcp, SIGNAL(closed()), this,
                        SLOT(on_device_disconnect()));
    Q_ASSERT(connected);

    connected = connect(evapTcp, SIGNAL(opened()), this,
                        SLOT(on_device_connect()));
    Q_ASSERT(connected);

    _popUpQueueMsgNotifier = new PopUp(this);

    connected = connect(this, SIGNAL(popupTextAdded(QString)), this,
                        SLOT(showPopUp(QString)));
    Q_ASSERT(connected);

    evapInterfaceList.append(evapTcp);
    emit curEvapInterfaceChanged(evapInterfaceList.first());
}

void MainWindow::on_processEvent()
{
//    QApplication::processEvents(QEventLoop::ExcludeSocketNotifiers);
}

void MainWindow::showPopUp(QString text)
{
    _popUpQueueMsgNotifier->setCanvasColor(QBrush(QColor(20,20,255,150)), Qt::SolidLine);
    _popUpQueueMsgNotifier->setPopupText(text);
    _popUpQueueMsgNotifier->show(1000);
}

void MainWindow::queueCommandsChanged(const QQueue<Command *> queue)
{
    Q_UNUSED(queue)
//    _commandsList.clear();
//    for(int i=1; i < queue.count(); i++) {
//       _commandsList.append(QString("%1 в очереди").arg(queue.at(i)->toString()));
//    }

//    QString curCmdStr = queue.first()->toString();
//    _commandsList.append(QString("=> %1").arg(curCmdStr));
//    emit popupTextAdded(_commandsList.join("<br>"));
}

void MainWindow::commandSent(const Command *cmd)
{
    Q_UNUSED(cmd)
//    emit popupTextAdded(cmd->toString());
}

void MainWindow::on_ErrorMessagesGot(QString msgText)
{
    UtilsForGui::ShowMessageInfo(statusBar(), msgText, UtilsForGui::MsgError, 3000);
    _popUpQueueMsgNotifier->setCanvasColor(QBrush(QColor(255,0,0,200)), Qt::SolidLine);
    _popUpQueueMsgNotifier->setPopupText(msgText);
    _popUpQueueMsgNotifier->show(3000);
}

void MainWindow::on_device_disconnect()
{
    tryToReconnect();

    _popUpQueueMsgNotifier->setCanvasColor(QBrush(QColor(255,0,0,180)), Qt::SolidLine);
    _popUpQueueMsgNotifier->setPopupText("Устройство отключено");
    _popUpQueueMsgNotifier->show(3000);
}

void MainWindow::on_device_connect()
{
    UtilsForGui::ShowMessageInfo(statusBar(), tr(""), UtilsForGui::MsgClear);

    ui->controlWidget->initResetCmd();
    timerReset.stop();
    timerReconnect.stop();

    ui->monitorWidget->startMonitor(TIMEOUT_CONTROL);
    ui->controlWidget->startControl(TIMEOUT_CONTROL);

    _popUpQueueMsgNotifier->setCanvasColor(QBrush(QColor(15,200,20,200)), Qt::SolidLine);
    _popUpQueueMsgNotifier->setPopupText("Устройство подключено");
    _popUpQueueMsgNotifier->show(3000);
}

void MainWindow::curEvapInterfaceChanged(EvapInterface *evapInterface)
{
    curEvapInterface = evapInterface;
}

void MainWindow::on_actionDebug_triggered()
{
    DebugWindow *form = new DebugWindow(curEvapInterface, ms, this);
    form->showNormal();
}

void MainWindow::tabExpanded(bool isExpanded)
{
    QRect rect = this->geometry();
    if (isExpanded)
        rect.setWidth(rect.width() + tabSize);
    else {
        rect.setWidth(rect.width() - tabSize);
    }
    this->setGeometry(rect);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    this->close();
    ms->stop();
}

void MainWindow::initUI()
{
    UtilsForGui::MoveWindowToCenter(this);

    ui->tabWidget->init(QSize(0, 32));
    ui->tabWidget->setMaximumWidth(tabSize);

    bool connected = false;
    connected = connect(ui->tabWidget, SIGNAL(expanded(bool)), this, SLOT(tabExpanded(bool)));
    Q_ASSERT(connected);

    ui->groupBox_AccidentNotification->setVisible(false);
    ui->monitorWidget->init(curEvapInterface, ms);  // begin the monitor service
    connected = connect(ui->monitorWidget, SIGNAL(showDamagedInfo(QStringList)), this,
            SLOT(showDamagedInfo(QStringList)));
    Q_ASSERT(connected);
    connected = connect(ui->monitorWidget, SIGNAL(nothingHappened()), this,
                        SLOT(clearAccidentList()));
    Q_ASSERT(connected);

    ui->tabWidget->setVisible(false);


    widgetIPconfig = new SetupIPConfigWidget ();
    widgetIPconfig->init(curEvapInterface, ms);
    widgetID = new SetIDWidget ();
    widgetID->init(curEvapInterface, ms);
    ui->controlWidget->init(curEvapInterface, ms);
}

void MainWindow::initStatusBar()
{
    idInfoWidget = new IdInfoWidget(curEvapInterface, ms, this);
    bool connected;
    connected = connect(idInfoWidget, SIGNAL(doubleClicked()), this,
                        SLOT(on_actionAbout_triggered()));
    Q_ASSERT(connected);
    connected = connect(idInfoWidget, SIGNAL(clicked()), this,
                        SLOT(on_actionAbout_triggered()));
    Q_ASSERT(connected);

    foreach (EvapInterface* ev, evapInterfaceList) {
        connected = connect(ev, SIGNAL(opened()), idInfoWidget, SLOT(updateIdInfo()));
        Q_ASSERT(connected);
    }

    setInterfaceWidget = new SetInterfaceWidget(this);
    connected = connect(setInterfaceWidget, SIGNAL(curEvapInterfaceChanged(EvapInterface*)),
            this, SLOT(curEvapInterfaceChanged(EvapInterface*)), Qt::DirectConnection);
    Q_ASSERT(connected);
    setInterfaceWidget->init(evapInterfaceList, ms);
    curEvapInterface = setInterfaceWidget->openDevice();

    statusBar()->addPermanentWidget(setInterfaceWidget);
    statusBar()->addPermanentWidget(idInfoWidget);
}

void MainWindow::on_actionChangeID_triggered()
{
    SetupDialog *dialog = new SetupDialog(widgetID,
                                          tr("занесение идентификационных данных"),
                                          this);
    connect(widgetID, SIGNAL(finished()), dialog, SLOT(close()));
    connect(widgetID, SIGNAL(finished()), idInfoWidget, SLOT(updateIdInfo()));
    dialog->show();
}

void MainWindow::on_action_ChangeIP_triggered()
{
    SetupDialog *dialog = new SetupDialog(widgetIPconfig,
                                          tr("смена базового IP-адреса, маски подсети и порта"),
                                          this);
    connect(widgetIPconfig, SIGNAL(finished()), dialog, SLOT(close()));
    dialog->show();

}

void MainWindow::showDamagedInfo(QStringList messgesList)
{
    if (messgesList.isEmpty()) {
        clearAccidentList();
        return;
    }

    ui->groupBox_AccidentNotification->setVisible(true);
    ui->plainTextEditDamageInfo->clear();
    int num = 1;
    foreach (QString text, messgesList) {
        ui->plainTextEditDamageInfo->appendPlainText(QString("%1) %2").
                                                     arg(num++).arg(text));
    }
}

void MainWindow::squeezeSize()
{
    QRect size = this->geometry();
    size.setWidth(0);
    this->setGeometry(size);
}

void MainWindow::clearAccidentList()
{
    ui->groupBox_AccidentNotification->setVisible(false);
    squeezeSize();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *dialog = new AboutDialog(curEvapInterface, ms, this);
    dialog->show();
}

void MainWindow::tryToReconnect()
{
    timerReconnect.start(5000);
}

void MainWindow::showResetMCStatus()
{
    UtilsForGui::ShowMessageInfo(statusBar(), trUtf8("идет перезагрузка ...  %1 сек.").
                                 arg(++resetTime),
                                 UtilsForGui::MsgInfo);
}

void MainWindow::startWaiting()
{
    resetTime = 0;
    timerReset.start(1000);
}

void MainWindow::initConnect()
{
    bool connected;
    connected = connect(&timerReconnect, SIGNAL(timeout()), setInterfaceWidget,
                        SLOT(openDevice()));
    Q_ASSERT(connected);
    connected = connect(&timerReset, SIGNAL(timeout()), this,
            SLOT(showResetMCStatus()));
    Q_ASSERT(connected);
}

void MainWindow::initControlWidget()
{
    bool connected;
    foreach (EvapInterface* ev, evapInterfaceList) {
        connected = connect(ev, SIGNAL(opened()), ui->controlWidget, SLOT(initUI()));
        Q_ASSERT(connected);
    }
    connected = connect(ui->controlWidget, SIGNAL(resetMC()), this,
            SLOT(tryToReconnect()));
    Q_ASSERT(connected);
    connected = connect(ui->controlWidget, SIGNAL(resetMC()), this,
            SLOT(startWaiting()));
    Q_ASSERT(connected);
}
