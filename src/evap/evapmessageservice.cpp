#include "evapmessageservice.h"
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include "../evap-protocol/cmd/evapcommand.h"
#include "../evap-protocol/cmd/nocommand.h"

EvapMessageService::EvapMessageService(QObject *parent) : QObject(parent),
    _isRunning(false)
{
}

EvapMessageService::~EvapMessageService()
{
    stop();
}

void EvapMessageService::start()
{
    if (isRunning())
        return;
    thread = new QThread();
    worker = new MessageWorker();

    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(mainloop()), Qt::DirectConnection);
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);

    _isRunning = true;
    thread->start();
}

void EvapMessageService::stop()
{
    if (worker == NULL || thread == NULL)
        return;
    _isRunning = false;
    worker->abort();
    thread->wait();

    qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();

    delete thread;
    delete worker;

    worker = NULL;
    thread = NULL;
}

bool EvapMessageService::isRunning()
{
    return _isRunning;
}

bool EvapMessageService::getAsync(Command *cmd)
{
    Q_ASSERT(((EvapCommand*)cmd)->typeCommand() == Command::GET);

    runCmdAsync(cmd);

    if (!cmd->status())
    {
        if (!cmd->errMsg().isEmpty())
            emit sigErrMsg(cmd->errMsg());
        return false;
    }
    return true;
}

bool EvapMessageService::getAsync(int code, QByteArray &data,
                             EvapInterface *eface, QString description)
{
    NoCommand cmd(Command::GET, code, data, description, eface);
    bool status = getAsync(&cmd);
    data = cmd.data();
    return status;
}

bool EvapMessageService::postAsync(int code, const QByteArray &data,
                                   EvapInterface *eface, QString description)
{
    NoCommand cmd(Command::POST, code, (QByteArray &)data, description, eface);
    return postAsync(&cmd);
}

bool EvapMessageService::postAsync(Command *cmd)
{
    Q_ASSERT(((EvapCommand*)cmd)->typeCommand() == Command::POST);

    runCmdAsync(cmd);

    if (!cmd->status() && !cmd->errMsg().isEmpty()) {
        emit sigErrMsg(cmd->errMsg());
    }
    return cmd->status();
}

void EvapMessageService::runCmdAsync(Command *cmd)
{
    qDebug() << cmd->repr() << "worker->runCmd(cmd);";
    if (worker == NULL) return;
    worker->runCmd(cmd);

    while (worker != NULL && worker->isBusy()) {
        emit eventLoop();
    }
}

MessageWorker *EvapMessageService::getWorker() const
{
    return worker;
}
