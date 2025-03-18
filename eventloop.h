#pragma once
#include <iostream>
#include "icommand.h"
#include "safequeue.h"
#include "istate.h"

class eventloop
{
public:
    eventloop(SafeQueue<ICommand *> *cmds, StateStatus *status);
    ~eventloop();

    SafeQueue<ICommand *> *cmds;
    StateStatus *status;

    void start(SafeQueue<ICommand *> *cmds, StateStatus *status);
    void execute(ICommand *cmd);
private:
    class eventloopP* imp;
};
