#ifndef MSGGETFANSPEED_H
#define MSGGETFANSPEED_H

#include "../evap-protocol/cmd/evapcommand.h"
#include <QVector>

class MsgGetFanSpeed : public EvapCommand
{
public:
    MsgGetFanSpeed(EvapInterface *eface);

    QVector<quint32> lower_compartment_fan;
    QVector<quint32> upper_compartment_fan;
    QVector<quint32> local_compartment_fan;


    // Command interface
public:
    virtual QString toString() const;

private:
    virtual QDataStream &output(QDataStream &stream) const;
    virtual QDataStream &input(QDataStream &stream);
};

#endif // MSGGETFANSPEED_H
