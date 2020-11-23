#include "command.h"

Command::Command(TypeCommand typeCommand, int code, QByteArray dataArray, EvapInterface *eface, QString text):
    _eface(eface), _code(code), _data(dataArray), _status(false),_errMsg(""),
    _isRunning(false), _typeCommand(typeCommand)
{
    parseData(dataArray);
    _text = QString("[%1, %2] (\"%3\")").arg(_code).
                                         arg(data().length()).
                                         arg(text);
}

Command::Command(TypeCommand typeCommand, int code, EvapInterface *eface, QString text):
    _eface(eface), _code(code), _status(false),_errMsg(""),
    _isRunning(false), _typeCommand(typeCommand)
{
    _text = QString("[%1, %2] (\"%3\")").arg(_code).
                                         arg(data().length()).
                                         arg(text);
}

QString Command::repr() const
{
    return _text;
}

int Command::code() const
{
    return _code;
}

QByteArray Command::data() const
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setByteOrder(QDataStream::LittleEndian);
    out << *this;
    return ba;
}

bool Command::status() const
{
    return _status;
}

bool Command::isRunning() const
{
    return _isRunning;
}

void Command::setIsRunning(bool isRunning)
{
    _isRunning = isRunning;
}

QString Command::errMsg() const
{
    return _errMsg;
}

EvapInterface *Command::eface() const
{
    return _eface;
}

Command::TypeCommand Command::typeCommand() const
{
    return  _typeCommand;
}

bool Command::executeCmd()
{
     _status = _eface->runAwait(this);
     return status();
}

bool Command::parseData(QByteArray &packet)
{
    QDataStream in(&packet, QIODevice::ReadOnly);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in.setByteOrder(QDataStream::LittleEndian);
    in >> *this;
    return true;
}

QByteArray Command::toArray() const
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setByteOrder(QDataStream::LittleEndian);

    out << quint32(code()) << quint32(0) << data();
    out.device()->seek(sizeof(quint32));
    out << quint32(ba.size() - int(sizeof(quint32))*2);

    return ba;
}

QDataStream &Command::output(QDataStream &stream) const
{
//    stream.writeRawData(data().data(), data().size());
    return stream;
}

QDataStream &Command::input(QDataStream &stream)
{
//    stream.readRawData(data().data(), data().size());
    return stream;
}

