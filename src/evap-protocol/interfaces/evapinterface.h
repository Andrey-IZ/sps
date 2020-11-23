#ifndef EVAPINTERFACE_H
#define EVAPINTERFACE_H

#include <QObject>
#include <QByteArray>

class Command;

class EvapInterface: public QObject
{
    Q_OBJECT

public:
    enum TypeEvapInterface {
        TCP,
        SERIALPORT
    };

    EvapInterface(TypeEvapInterface typeEvap, QObject *parent = 0):
        QObject (parent), _typeInterface(typeEvap) {}
    virtual ~EvapInterface() {}

    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool isOpen() = 0;

    virtual bool runAwait(Command *cmd) = 0;
    virtual bool runPostAwait(quint32 code, QByteArray data) = 0;
    virtual QByteArray runGetAwait(quint32 code, int length) = 0;

    virtual QString toString() = 0;

    TypeEvapInterface typeInterface() const {
        return _typeInterface;
    }

signals:
    void opened();
    void closed();

    void configureChanged();

    void msgSent(quint32 code, QByteArray data);
    void msgRecv(quint32 code, QByteArray data);
    void error(QString message);

private:
    TypeEvapInterface _typeInterface;
};

#endif // EVAPINTERFACE_H

