#pragma once
#include <iostream>
#include "icommand.h"
#include "safequeue.h"

class ExceptionHandler
{
public:
    ExceptionHandler(ICommand* cmd, std::exception ex);
    ~ExceptionHandler();

    ICommand* cmd() const;
    std::exception ex() const;

    void executeWrite(SafeQueue<ICommand *> *cmd, std::exception ex);
    void executeRepeat(ExceptionHandler* handler, SafeQueue<ICommand *> *cmd, std::exception ex);
private:
    class ExceptionHandlerP* imp;
};
