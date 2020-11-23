#include "msggetidinfo.h"

#include <QStringList>

MsgGetIdInfo::MsgGetIdInfo(EvapInterface *eface):
    EvapCommand(EvapCommand::GET, READ_ID,
                QString(QObject::tr("считывание идентификационной информации")),
                eface),
    id(0),  moduleNumber(0), serialNumber(0),
    version(0), crc(0)
{
}

MsgGetIdInfo::~MsgGetIdInfo()
{
}

QString MsgGetIdInfo::versionToStr() const
{
    return QString("%1.%2.%3").arg(QString::number((version >> 16) & 0xF))
                              .arg(QString::number((version >> 8) & 0xF))
                              .arg(QString::number((version) & 0xF));
}

QString MsgGetIdInfo::idToStr() const
{
    return QString::fromAscii((char*)&id, sizeof(id));
}

QDataStream &MsgGetIdInfo::output(QDataStream &stream) const
{
    stream << id << moduleNumber << serialNumber
           << releaseDate << version << crc;
    return stream;
}

QDataStream &MsgGetIdInfo::input(QDataStream &stream)
{
    stream >> id >> moduleNumber >> serialNumber
           >> releaseDate >> version >> crc;
    return stream;
}

QStringList MsgGetIdInfo::toInfoList() const
{
    QStringList str;
    str += QString("Код фирмы: %1").arg(idToStr());
    str += QString("децимальный номер: %1").arg(moduleNumber);
    str += QString("серийный номер: %1").arg(QString::number(serialNumber));
    str += QString("дата изготовления: %1").arg(releaseDate.toString());
    str += QString("версия ПО: %1").arg(versionToStr());
    str += QString("контрольная сумма ПО (CRC32): 0x%1").arg(QString::number(crc, 16).toUpper());
    return str;
}

QString MsgGetIdInfo::toString() const
{
    toInfoList().insert(0, repr());
    return toInfoList().join("\n");
}

