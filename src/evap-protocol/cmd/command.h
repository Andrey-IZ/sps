#ifndef COMMAND_H
#define COMMAND_H

#include <QDataStream>
#include "../interfaces/evapinterface.h"

class Command
{
public:
    enum TypeCommand {
        POST, GET
    };

    explicit Command(TypeCommand typeCommand, int _code, QByteArray _data,
                     EvapInterface *eface, QString text);
    Command(TypeCommand typeCommand, int code, EvapInterface *eface, QString text);
    virtual ~Command() {}


    enum CommandListType {
        READ_ID                 = 0,
        RESET_CONTROLLER        = 0,
        READ_BLOCK_ADDRESS      = 0,
        READ_INDICATOR_STATUS   = 0,
        TURN_ON_INDICATORS      = 0,
        SELF_CONTROL            = 0,
        READ_ACCIDENT_REGISTER  = 0,
        SYSRESET                = 0,
        SET_FAN_SPEED           = 0,
        GET_FAN_SPEED           = 0,
        MONITOR_FAN_SPEED       = 0,
        GET_DETERMINED_FAN_SPEED= 0,
        TURN_ON_POWER_SUPPLY    = 0,
        TURN_ON_24V             = 0,
        TURN_ON_12V_1           = 0,
        TURN_ON_12V_2           = 0,
        MONITOR_SPS             = 0,
        MONITOR_ZVS_12V         = 0,
        READ_BCM_STATUS         = 0,
        MONITOR_MPFC            = 0,
        GET_SPS_TEMP            = 0,
        ADC_CALIBRATION         = 0,
        CHANGE_IP_CONFIG        = 0,
        SET_ID                  = 0,
        OK                      = 0
    };

    friend QDataStream& operator << (QDataStream &stream, const Command &msg) {
        return msg.output(stream);
    }

    friend QDataStream& operator >> (QDataStream &stream, Command &msg) {
        return msg.input(stream);
    }

    bool parseData(QByteArray &packet);

    QByteArray toArray() const;

    virtual bool execute(int delay = 0) = 0;
    virtual QString toString() const = 0;
    QString repr() const;

    int code() const;
    QByteArray data() const;

    bool status() const;

    bool isRunning() const;
    void setIsRunning(bool isRunning);

    virtual QString errMsg() const;
    EvapInterface *eface() const;

    TypeCommand typeCommand() const;

protected:
    EvapInterface *_eface;
    int _code;
    QByteArray _data;
    bool _status;
    QString _text;
    QString _errMsg;
    bool _isRunning;

    bool executeCmd();

private:
    TypeCommand _typeCommand;

private:
    virtual QDataStream &output(QDataStream &stream) const;
    virtual QDataStream &input(QDataStream &stream);
};

#endif // COMMAND_H
