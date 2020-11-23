#ifndef MESSAGEWORKER_H
#define MESSAGEWORKER_H

#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include "evap_global.h"
#include "../evap-protocol/cmd/command.h"

class EVAPSHARED_EXPORT MessageWorker : public QObject
{
    Q_OBJECT
public:
    explicit MessageWorker(QObject *parent = 0);
    ~MessageWorker();

    void abort();
    bool isBusy();

public slots:
    void mainloop();
    void runCmd(Command *cmd);

signals:
    void finished();
    void commandHandled();
    void queueCommandsChanged(const QQueue<Command*> queue);
    void commandSent(const Command* cmd);

private:
    QQueue<Command*> _commandQueue;
    bool _abort;
    bool _isRunning;
    /**
     * @brief Protects access to #_abort
     */
    QMutex mutex;
    /**
     * @brief Condition is used to wait for a new request to be called in the #mainLoop()
     */
    QWaitCondition condition;

};

#endif // MESSAGEWORKER_H
