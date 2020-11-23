#ifndef NOCOMMAND_H
#define NOCOMMAND_H

#include "../evap-protocol/cmd/evapcommand.h"

class NoCommand : public EvapCommand
{
public:
    NoCommand(TypeCommand typeCommand, int code, QByteArray &data, QString description, EvapInterface *eface);

    QByteArray byteArray;

    // Command interface
public:
    virtual QString toString();

private:
    virtual QDataStream &output(QDataStream &stream) const;
    virtual QDataStream &input(QDataStream &stream);
};

#endif // NOCOMMAND_H
