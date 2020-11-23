#include "okcommand.h"

#include <QDataStream>

OkCommand::OkCommand(EvapInterface *eface):
    Command(Command::GET, 901, eface, QString("Чтение кода ошибки")),
    statusCode(0)
{
}

QString OkCommand::toString() const
{
    return QString("%1 Сообщение: \"%2\"}").arg(Command::repr()).arg(errMsg());
}

bool OkCommand::execute(int delay)
{
    Q_UNUSED(delay)
    return executeCmd() && statusCode == 1;
}


QString OkCommand::errMsg() const
{
    QString msg;
    switch (static_cast<ErrorMsg>(statusCode)) {
        case OkCommand::No:
            msg = QString("Ошибок нет");
            break;
        case OkCommand::MsgBroken:
            msg = QString("Нарушение формата команды");
            break;
        case OkCommand::MsgSizeDispariry:
            msg = QString("Несоответствие размера данных");
            break;
        case OkCommand::MsgHasNotBeenRecognized:
            msg = QString("Команда не опознана");
            break;
        case OkCommand::MsgFirmwareDoesntMatchModule:
            msg = QString("Прошивка не соответствует модулю");
            break;
        case OkCommand::MsgDataError:
            msg = QString("Недопустимое значение параметра, ошибка данных");
            break;
        case OkCommand::MsgFlashDataError:
            msg = QString("Повреждена информация во флэш-памяти");
            break;
        default:
            msg = QString("неверное значение");
    }
    return msg;
}
