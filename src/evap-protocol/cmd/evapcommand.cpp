#include "evapcommand.h"
#include "okcommand.h"

#include <unistd.h>
#include <QStringList>

EvapCommand::EvapCommand(Command::TypeCommand typeCommand, int code, QString description, EvapInterface *eface):
    Command (typeCommand, code, QByteArray(), eface, description)
{
}

EvapCommand::EvapCommand(Command::TypeCommand typeCommand, int code, QByteArray &data, QString description, EvapInterface *eface):
    Command (typeCommand, code, data, eface, description)
{
}

bool EvapCommand::execute(int delay)
{
    OkCommand okCmd(_eface);
    _status = false;

    if (!okCmd.execute())
    {
        _errMsg = okCmd.errMsg();
        return okCmd.status();
    }

    if (!_eface->runAwait(this))
        return false;

    if (delay > 0)
        usleep(quint32(1000 * delay));

    if (!okCmd.execute())
    {
        _errMsg = okCmd.errMsg();
        return okCmd.status();
    }

    _status = true;
    return _status;
}

QString EvapCommand::toString() const
{
    QStringList str;
    str += repr();
    return str.join("\n");
}
