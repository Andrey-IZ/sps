#include "nocommand.h"
#include <QDateTime>
#include <QStringList>
#include <unistd.h>

NoCommand::NoCommand(Command::TypeCommand typeCommand, int code, QByteArray &data,
                     QString description, EvapInterface *eface):
    EvapCommand(typeCommand, code, description, eface),
    byteArray(data)
{
}

QString NoCommand::toString()
{
    QStringList str;
    str += repr();
    QStringList bytesHex;
    for (int i = 0; i < byteArray.size(); ++i) {
        bytesHex.append(QString("%1").arg(QString::number(quint8(byteArray.at(i)), 16)).
                        rightJustified(2, QLatin1Char('0')));
    }
    str += QString("Данные: 0x%1").arg(bytesHex.join(""));

    return str.join("\n");
}

QDataStream &NoCommand::output(QDataStream &stream) const
{
    stream.writeRawData(byteArray.data(), byteArray.size());
    return stream;
}

QDataStream &NoCommand::input(QDataStream &stream)
{
    stream.readRawData(byteArray.data(), byteArray.size());
    return stream;
}
