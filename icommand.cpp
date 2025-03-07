#include <iostream>
#include <string>
#include "icommand.h"

class MacroCommandP
{
public:
    std::list<ICommand*> cmds;

    MacroCommandP(std::list<ICommand*> cmds) :
        cmds(cmds)
    {
    }
};

MacroCommand::MacroCommand(std::list<ICommand*> cmds) :
    imp(new MacroCommandP(cmds))
{
}

MacroCommand::~MacroCommand() { delete imp;}

void MacroCommand::execute()
{
    std::cout << "MacroCommand" << std::endl;
    if(imp->cmds.empty())
        throw std::runtime_error ("Сommand list is empty");
    for(ICommand* i : imp->cmds)
    {
        try
        {
            i->execute();
        } catch (...) {
            throw std::runtime_error ("Pass the error up");
        }
    }
}

int MacroCommand::get_Id_parent()
{
    int result = imp->cmds.front()->get_Id_parent();
    return result;
}
