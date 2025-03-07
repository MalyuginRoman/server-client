#ifndef ICOMMAND_H
#define ICOMMAND_H
#include <iostream>
#include <list>
#include <map>
#include <functional>
#include "order.h"

class objectVector;
class object;
struct system_okr;

enum CommandCodes
{
    CommandMove = 1,
    CommandRotate = 2,
    CommandCheck = 3,
    CommandBurn = 4,
    CommandLoger = 5,
    CommandEmpty = 6,
    CommandHardStop = 7,
    CommandSoftStop = 8,
    CommandInternet = 9,
    CommandMoveTo = 10,
    CommandRun = 11,
    CommandAddLast = 12,
    CommandStart = 13,
    CommandStop = 14,
    CommandShoot = 15,
    CommandMacro = 100
};

class ICommand
{
public:
    virtual int get_Id_cmd() = 0;
    virtual int get_Id_parent() = 0;
    virtual void execute() = 0;
};

class MoveCommand : public ICommand
{
public:
    MoveCommand(std::map<int, system_okr>* p_map_a,
                std::map<int, system_okr>* p_map_b,
                object* obj);
    ~MoveCommand();

    object* obj() const;
    std::map<int, system_okr>* p_map_a() const;
    std::map<int, system_okr>* p_map_b() const;

    int get_Id_cmd();
    int get_Id_parent();
    void execute();

private:
    class MoveCommandP* imp;
};

class CheckPositionCommand
{
public:
    void execute(std::map<int, system_okr>* p_map, object* obj);
};

class RotateCommand : public ICommand
{
public:
    RotateCommand(object* obj);
    ~RotateCommand();

    object* obj() const;

    int get_Id_cmd();
    int get_Id_parent();
    void execute();

private:
    class RotateCommandP* imp;
};

class CheckCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandCheck;
    }
    int get_Id_parent()
    {
        return -9999;
    }
    void execute()
    {
        std::cout << "CheckCommand" << std::endl;
    }
};

class BurnCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandBurn;
    }
    int get_Id_parent()
    {
        return -9999;
    }
    void execute()
    {
        std::cout << "BurnCommand" << std::endl;
    }
};

class LogerCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandLoger;
    }
    int get_Id_parent()
    {
        return -9999;
    }
    void execute()
    {
        std::cout << "LogerCommand" << std::endl;
    }
};

class EmptyCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandEmpty;
    }
    int get_Id_parent()
    {
        return -9999;
    }
    void execute()
    {
        std::cout << "EmptyCommand" << std::endl;
    }
};

class HardStopCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandHardStop;
    }
    int get_Id_parent()
    {
        return -9999;
    }
    void execute()
    {
        std::cout << "HardStopCommand" << std::endl;
    }
};

class SoftStopCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandSoftStop;
    }
    int get_Id_parent()
    {
        return -9999;
    }
    void execute()
    {
        std::cout << "SoftStopCommand" << std::endl;
    }
};

class InternetCommand : public ICommand
{
public:
    InternetCommand(objectVector* vect, object* obj, order *order_cmd,
                    std::list<ICommand*> *cmds);
    ~InternetCommand();

    objectVector* vect;
    object* obj;
    order *order_cmd;
    std::list<ICommand*> cmds;

    int get_Id_cmd()
    {
        return CommandInternet;
    }
    void execute();
    int get_Id_parent();

    bool create();
private:
    class InternetCommandP* imp;
};

class MoveToCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandMoveTo;
    }
    int get_Id_parent()
    {
        return -9999;
    }
    void execute()
    {
        std::cout << "MoveToCommand" << std::endl;
    }
};

class RunCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandRun;
    }
    int get_Id_parent()
    {
        return -9999;
    }
    void execute()
    {
        std::cout << "RunCommand" << std::endl;
    }
};

class AddLastCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandAddLast;
    }
    int get_Id_parent()
    {
        return -9999;
    }
    void execute()
    {
        std::cout << "AddLastCommand" << std::endl;
    }
};

class StartMotion : public ICommand
{
public:
    StartMotion(object* obj, double du);
    ~StartMotion();

    object* obj() const;
    double du() const;

    int get_Id_cmd();
    int get_Id_parent();
    void execute();

private:
    class StartMotionP* imp;
};

class StopMotion : public ICommand
{
public:
    StopMotion(object* obj);
    ~StopMotion();

    object* obj() const;

    int get_Id_cmd();
    int get_Id_parent();
    void execute();

private:
    class StopMotionP* imp;
};

class ShootCommand : public ICommand
{
public:
    ShootCommand(objectVector* vect, object* obj);
    ~ShootCommand();

    objectVector* vect() const;
    object* obj() const;

    int get_Id_cmd();
    int get_Id_parent();
    void execute();

private:
    class ShootCommandP* imp;
};

class MacroCommand : public ICommand
{
public:
    int get_Id_cmd()
    {
        return CommandMacro;
    }
    int get_Id_parent();
    MacroCommand(std::list<ICommand*> cmds);
    ~MacroCommand();
    std::list<ICommand*> cmds;
    void execute();
private:
    class MacroCommandP* imp;
};
#endif // ICOMMAND_H
