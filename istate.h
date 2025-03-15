#ifndef ISTATE_H
#define ISTATE_H

#include <iostream>
#include <string>
#include "icommand.h"

class State;
class StateStatus;
class DefaultState;
class MoveToState;
class ExitState;

class State
{
    std::string status;
public:
    ICommand* cmd;
    State(const std::string& status, ICommand* cmd)
        : status(status), cmd(cmd) {}
    std::string GetStatus()
    {
        return status;
    }
    ICommand* GetCmd()
    {
        return cmd;
    }
    virtual void execute(StateStatus*, ICommand*) = 0;
};

class StateStatus
{
private:
    State* state;
    ICommand* cmd;
public:
    StateStatus(State* state, ICommand* cmd)
        : state(state), cmd(cmd) {}

    void execute(ICommand* cur_cmd)
    {
        std::cout << "Using " << state->GetStatus() << " state" << std::endl;
        state->execute(this, cur_cmd);
    }
    void SetState(State* s)
    {
        std::cout << "Changing state from " << state->GetStatus()
                  << " to " << s->GetStatus() << "..." << std::endl;
        delete state;
        state = s;
    }
    State* GetState()
    {
        return state;
    }
    ~StateStatus()
    {
        delete state;
        delete cmd;
    }
};

class DefaultState : public State
{
public:
    DefaultState() : State("Default", new EmptyCommand) {}
    virtual void execute(StateStatus *state, ICommand *cmd);
};

class MoveToState : public State
{
public:
    MoveToState() : State("MoveTo", new EmptyCommand) {}
    virtual void execute(StateStatus *state, ICommand *cmd);
};

class ExitState : public State
{
public:
    ExitState() : State("Exit", nullptr) {}
    virtual void execute(StateStatus *state, ICommand *cmd);
};

#endif // ISTATE_H
