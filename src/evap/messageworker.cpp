#include "messageworker.h"
#include <QThread>
#include <unistd.h>
#include <QDebug>

MessageWorker::MessageWorker(QObject *parent) : QObject (parent),
    _abort(false), _isRunning(false)
{
}

MessageWorker::~MessageWorker()
{
}

void MessageWorker::runCmd(Command *cmd)
{
    QMutexLocker locker(&mutex);
    _isRunning = true;
    cmd->setIsRunning(true);
    _commandQueue.enqueue(cmd);
    condition.wakeOne();
}

void MessageWorker::mainloop()
{
    forever
    {
        mutex.lock();
        if (!_abort && _commandQueue.isEmpty())
        {
            _isRunning = false;
            condition.wait(&mutex);
        }

        if (_abort) {
            qDebug()<<"Aborting worker mainLoop in Thread "<<thread()->currentThreadId();
            mutex.unlock();
            _isRunning = false;
            emit finished();
            return;
        }
        _isRunning = true;

        if (_commandQueue.count() > 1) {
            emit queueCommandsChanged(_commandQueue);
        }

        Command* cmd = _commandQueue.dequeue();
        mutex.unlock();

        if (cmd->execute())
            emit commandSent(cmd);

        mutex.lock();
        cmd->setIsRunning(false);
        mutex.unlock();
        emit commandHandled();
    }
}

void MessageWorker::abort()
{
    QMutexLocker locker(&mutex);
    qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
    _abort = true;
    condition.wakeOne();
}

bool MessageWorker::isBusy()
{
    return _isRunning;
}
