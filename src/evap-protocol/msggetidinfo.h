#ifndef MSGGETIDINFO_H
#define MSGGETIDINFO_H

#include <QDate>
#include "../evap-protocol/cmd/evapcommand.h"

class MsgGetIdInfo : public EvapCommand
{
public:
    MsgGetIdInfo(EvapInterface *eface);
    ~MsgGetIdInfo();

    quint32 id;           //!< Код фирмы
    quint32 moduleNumber; //!< децимальный номер
    quint32 serialNumber; //!< серийный номер
    QDate releaseDate;    //!< дата изготовления
    quint32 version;      //!< версия ПО
    quint32 crc;          //!< контрольная сумма ПО (CRC32)

    //!
    //! \brief versionToStr
    //! \return
    //!
    QString versionToStr() const;

    //!
    //! \brief idToStr
    //! \return
    //!
    QString idToStr() const;
    QStringList toInfoList() const;

    // EvapCommand interface
private:
    QDataStream &output(QDataStream &stream) const;
    QDataStream &input(QDataStream &stream);

public:
    virtual QString toString() const;
};

#endif // MSGGETIDINFO_H
