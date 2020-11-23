#include "msggetfanspeed.h"

#include <QStringList>

MsgGetFanSpeed::MsgGetFanSpeed(EvapInterface *eface):
    EvapCommand(EvapCommand::GET, GET_FAN_SPEED,
                QString(QObject::tr("измерение скорости вентиляторов")), eface)
{
    lower_compartment_fan.fill(0, 3);
    upper_compartment_fan.fill(0, 3);
    local_compartment_fan.fill(0, 6);
}

QString MsgGetFanSpeed::toString() const
{
    QStringList str;
    str += repr();

    str += QString("вентиляторы нижнего отсека:");
    foreach(quint32 value , lower_compartment_fan)
        str += " " + QString::number(value) + ",";

    str += QString("вентиляторы верхнего отсека:");
    foreach(quint32 value , upper_compartment_fan)
        str += " " + QString::number(value) + ",";

    str += QString("локальные вентиляторы:");
    foreach(quint32 value , local_compartment_fan)
        str += " " + QString::number(value) + ",";

    return str.join("\n");
}

QDataStream &MsgGetFanSpeed::output(QDataStream &stream) const
{
    for (int i = 0; i < lower_compartment_fan.count();i++)
        stream << lower_compartment_fan[i];
    for (int i = 0; i < upper_compartment_fan.count();i++)
        stream << upper_compartment_fan[i];
    for (int i = 0; i < local_compartment_fan.count();i++)
        stream << local_compartment_fan[i];
    return stream;
}

QDataStream &MsgGetFanSpeed::input(QDataStream &stream)
{
    for (int i = 0; i < lower_compartment_fan.count();i++)
        stream >> lower_compartment_fan[i];
    for (int i = 0; i < upper_compartment_fan.count();i++)
        stream >> upper_compartment_fan[i];
    for (int i = 0; i < local_compartment_fan.count();i++)
        stream >> local_compartment_fan[i];
    return stream;
}
