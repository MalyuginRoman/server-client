#include "istate.h"

void DefaultState::execute(StateStatus *state, ICommand *cmd)
{
    if(cmd->get_Id_cmd() == CommandHardStop)
    {
        state->SetState(new ExitState());
        std::cout << "Command lost after hard stop: ";
    }
    else if(cmd->get_Id_cmd() == CommandSoftStop)
    {
        state->SetState(new MoveToState());
        std::cout << "Command carried after soft stop: ";
    }
}

void MoveToState::execute(StateStatus *state, ICommand *cmd)
{
    if(cmd->get_Id_cmd() == CommandRun)
    {
        state->SetState(new DefaultState());
        std::cout << "Return to default state";
    }
}

void ExitState::execute(StateStatus *state, ICommand *cmd)
{
    if(cmd->get_Id_cmd() == CommandRun)
    {
        state->SetState(new DefaultState());
        std::cout << "Return to default state";
    }
}
