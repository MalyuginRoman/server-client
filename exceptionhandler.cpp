#include "exceptionHandler.h"
#include "icommand.h"

class ExceptionHandlerP
{
public:
    ICommand* cmd;
    std::exception ex;
    ExceptionHandlerP(ICommand* cmd, std::exception ex) :
        cmd(nullptr),
        ex(ex)
    {
    }
};

ExceptionHandler::ExceptionHandler(ICommand* cmd, std::exception ex) :
    imp(new ExceptionHandlerP(cmd, ex))
{
}

ExceptionHandler::~ExceptionHandler() { delete imp; }

void ExceptionHandler::executeRepeat(ExceptionHandler* handler, SafeQueue<ICommand *> *cmd, std::exception ex)
{
    std::cout << "Repeat execute of ExceptionHandler" << ex.what() << std::endl;
    cmd->push(cmd->front());
    cmd->pop();
    try {
        cmd->front()->execute();
        cmd->pop();
    } catch( std::exception ex1) {
        handler->executeWrite(cmd, ex1);
    }
}

void ExceptionHandler::executeWrite(SafeQueue<ICommand *> *cmd, std::exception ex)
{
    std::cout << "Write execute of ExceptionHandler" << ex.what() << std::endl;
    LogerCommand *cmd_loger = new LogerCommand;
    cmd_loger->execute();
    cmd->pop();
}
