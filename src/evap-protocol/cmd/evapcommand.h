#ifndef EVAPCOMMAND_H
#define EVAPCOMMAND_H

#include "command.h"

class EvapCommand: public Command
{
public:

    explicit EvapCommand(Command::TypeCommand typeCommand, int code, QString description,EvapInterface *eface);
    EvapCommand(Command::TypeCommand typeCommand, int code, QByteArray &data, QString description, EvapInterface *eface);
    virtual ~EvapCommand() {}

    // Command interface
public:
    virtual bool execute(int delay = 0);
    virtual QString toString() const;
};

#endif // EVAPCOMMAND_H
