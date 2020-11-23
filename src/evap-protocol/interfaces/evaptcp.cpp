#include "evaptcp.h"
#include <unistd.h>
#include <cmd/command.h>


EvapTCP::EvapTCP(QObject *parent): EvapInterface(EvapInterface::TCP, parent),
    socket(NULL), _isOpen(false)
{
}

void EvapTCP::closeEvap()
{
    if (socket !=NULL)
    {
        socket->abort();
        socket->close();
        delete socket;
        socket = NULL;
    }
}

EvapTCP::~EvapTCP()
{
    closeEvap();
}

bool EvapTCP::runPostAwait(quint32 code, QByteArray data)
{
    if (!isOpen()) {
        emit error(trUtf8("код=%1, устройство не подключено (%2)").arg(code).arg(toString()));
        return false;
    }

    // Отправка команды на чтение данных
    QByteArray cmd;
    QDataStream out(&cmd, QIODevice::WriteOnly);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setByteOrder(QDataStream::LittleEndian);

    if (data.length() <= int(sizeof(quint32))) {
        QByteArray ba(sizeof(quint32), 0);
        ba.insert(0, data);
        ba.resize(sizeof(quint32));
        data = ba;
    }

    out << quint32(code) << quint32(data.length());
    out.writeRawData(data.data(), data.length());

    socket->write(cmd);
    socket->waitForBytesWritten(2000);

    emit msgSent(code, cmd);

    return true;
}

QByteArray EvapTCP::runGetAwait(quint32 code, int length)
{
    if (!isOpen()) {
        return QByteArray();
    }

    socket->readAll();   // Очистка буферов порта

    // Отправка команды на чтение данных
    QByteArray cmd;
    QDataStream out(&cmd, QIODevice::WriteOnly);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setByteOrder(QDataStream::LittleEndian);

    out << quint32(code) << quint32(length);

    emit msgSent(code, cmd);

    socket->write(cmd);
    socket->waitForBytesWritten(2000);

    // Ожидание и проверка прихода прочитанных данных
    if (!socket->waitForReadyRead(2000) ) {
        emit error(trUtf8("код=%1, Таймаут ожидания приёма пакета истёк").arg(code));
        return QByteArray();
    }

    if (socket->bytesAvailable() < length + int(sizeof (quint32))) {
         emit error(trUtf8("код=%1, Неверный формат данных пакета: %2").arg(code).
                    arg(socket->bytesAvailable()));
        return QByteArray();
    }

    // Чтение прочитанных данных
    QByteArray answer = socket->readAll();
//    while (socket->bytesAvailable()  > answer.size())
//    {
//        answer += socket->readAll();
//        qDebug() << "answer" << answer.toHex();
//        break;
//    }

    QDataStream in(&answer, QIODevice::ReadOnly);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in.setByteOrder(QDataStream::LittleEndian);

    quint32 retCode;
    in >> retCode;

    QByteArray retData(length, '0');
//    retData.resize(int(length));
    in.readRawData((char*)retData.data(), int(length));

    if (in.status()!=QDataStream::Ok)
    {
        emit error(trUtf8("Статус DataStream: %1").arg(in.status()));
        return QByteArray();
    }

    // Проверка длины пакета
    if (retData.size() != length)
    {
        emit error(trUtf8("код=%1, Ошибка при чтении пакета, длина=%2, прочитано=%3").arg(code).
                   arg(length).arg(retData.length()));
        return QByteArray();
    }

    if (code != retCode) {
        emit error(trUtf8("код=%1, Коды пакетов не равны, %1 != %2").arg(code).arg(retCode));
        return QByteArray();
    }

    if (length != int(retData.length())) {
        emit error(trUtf8("код=%1, Длины пакетов не равны, %2 != %3; %4").arg(retCode).
                   arg(length).arg(retData.length()).arg(answer.length()));
        return QByteArray();
    }

    emit msgRecv(retCode, answer);
    return retData;
}

bool EvapTCP::open()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));

    socket->connectToHost(address, quint16(port));
    if (socket->waitForConnected(2000))
    {
        qDebug("Connected!");
        _isOpen = true;
        emit opened();
        return true;
    }
    _isOpen = false;
    return false;
}

bool EvapTCP::isOpen()
{
    if (socket != NULL)
        return socket->isOpen() && socket->isValid() && _isOpen;
    return false;
}

bool EvapTCP::close()
{
    if (isOpen())
        socket->close();
    closeEvap();
    return false;
}

QString EvapTCP::toString()
{
    return QString("tcp://%1:%2").arg(address).arg(port);
}

void EvapTCP::on_connected()
{
    qDebug("Event: Connected!");
//    _isOpen = true;
//    emit opened();
}

void EvapTCP::on_disconnected()
{
    _isOpen = false;
    emit closed();
}

void EvapTCP::setupTCP(const QString &address, const int port)
{
    this->address = address;
    this->port = port;
    emit configureChanged();
}

bool EvapTCP::runAwait(Command *cmd)
{
    if (cmd->typeCommand() == Command::GET)
    {
        QByteArray data = runGetAwait(quint32(cmd->code()), cmd->data().length());
        if (data.length() != cmd->data().length())
            return false;
        return cmd->parseData(data);
    }

    if (cmd->typeCommand() == Command::POST)
    {
        return runPostAwait(quint32(cmd->code()), cmd->data());
    }

    return false;
}
