#ifndef EVAPTCP_H
#define EVAPTCP_H

#include <QTcpSocket>
#include "evapinterface.h"

class EvapTCP: public EvapInterface
{
    Q_OBJECT
public:
    EvapTCP(QObject *parent = 0);
    ~EvapTCP();

    // EvapInterface interface
public:
    virtual bool runAwait(Command *cmd);
    virtual bool runPostAwait(quint32 code, QByteArray data);
    virtual QByteArray runGetAwait(quint32 code, int length);
    virtual bool open();
    virtual bool isOpen();
    virtual bool close();
    virtual QString toString();

    void setupTCP(const QString &address, const int port);

private slots:
    void on_connected();
    void on_disconnected();

private:
    QTcpSocket *socket;
    QString address;
    int port;
    bool _isOpen;

    void closeEvap();
};

#endif // EVAPTCP_H
