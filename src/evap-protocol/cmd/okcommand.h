#ifndef OKCOMMAND_H
#define OKCOMMAND_H

#include "command.h"

class OkCommand: public Command
{
public:
    OkCommand(EvapInterface *eface);

    enum ErrorMsg {
        No = 1,
        MsgBroken = 51,
        MsgSizeDispariry = 52,
        MsgHasNotBeenRecognized = 53,
        MsgFirmwareDoesntMatchModule = 54,
        MsgDataError = 55,
        MsgFlashDataError = 503,
    };

    quint32 statusCode;
    // Command interface
public:
    virtual QString toString() const;
    virtual bool execute(int delay = 0);

    // Command interface
private:
    QDataStream &output(QDataStream &stream) const {
        stream << statusCode;
        return stream;
    }
    QDataStream &input(QDataStream &stream)  {
        stream >> statusCode;
        return stream;
    }

    // Command interface
public:
    virtual QString errMsg() const;
};

#endif // OKCOMMAND_H
