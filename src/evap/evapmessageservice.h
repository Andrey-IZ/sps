#ifndef EVAPMESSAGESERVICE_H
#define EVAPMESSAGESERVICE_H
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include "evap_global.h"
#include "../evap-protocol/cmd/evapcommand.h"
#include "../evap-protocol/interfaces/evapinterface.h"
#include "messageworker.h"

class EVAPSHARED_EXPORT EvapMessageService : public QObject
{
    Q_OBJECT
public:
    explicit EvapMessageService(QObject *parent = 0);
    ~EvapMessageService();

    void start();
    void stop();

    bool isRunning();

    bool getAsync(int code, QByteArray &data, EvapInterface *eface, QString description);
    bool getAsync(Command *cmd);
    bool postAsync(int code, const QByteArray &data, EvapInterface *eface, QString description);
    bool postAsync(Command *cmd);
    MessageWorker *getWorker() const;

signals:
    void sigErrMsg(QString text);
    void eventLoop();

public slots:
private:
    QThread *thread;
    MessageWorker *worker;

    bool _isRunning;
    void runCmdAsync(Command *cmd);
};

#endif // EVAPMESSAGESERVICE_H
