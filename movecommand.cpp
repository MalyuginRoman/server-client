#include <iostream>
#include "icommand.h"
#include "object.h"
#include "objposition.h"

bool isXY(std::map<int, system_okr> *p_map, object* obj, int j)
{
    bool isX = ((p_map->at(j).XY.Xmin - obj->place().placeX) < eps) &&
               ((obj->place().placeX - p_map->at(j).XY.Xmax) < eps);
    bool isY = ((p_map->at(j).XY.Ymin - obj->place().placeY) < eps) &&
               ((obj->place().placeY - p_map->at(j).XY.Ymax) < eps);
    return (isX && isY);
}

bool isnXY(std::map<int, system_okr> *p_map, object* obj, int j)
{
    bool isX = ((p_map->at(j).XY.Xmin - obj->place().placeX) < eps) &&
               ((obj->place().placeX - p_map->at(j).XY.Xmax) < eps);
    bool isY = ((p_map->at(j).XY.Ymin - obj->place().placeY) < eps) &&
               ((obj->place().placeY - p_map->at(j).XY.Ymax) < eps);
    return (!isX || !isY);
}

class MoveCommandP
{
public:
    object* obj;
    std::map<int, system_okr>* p_map_a;
    std::map<int, system_okr>* p_map_b;

    MoveCommandP(std::map<int, system_okr>* p_map_a,
                 std::map<int, system_okr>* p_map_b,
                 object* obj) :
        p_map_a(p_map_a), p_map_b(p_map_b), obj(obj)
    {
    }
};

MoveCommand::MoveCommand(std::map<int, system_okr>* p_map_a,
                         std::map<int, system_okr>* p_map_b,
                         object* obj) :
    imp(new MoveCommandP(p_map_a, p_map_b, obj))
{}

MoveCommand::~MoveCommand() { delete imp;}

void MoveCommand::execute()
{
    CheckPositionCommand* cmd_search = nullptr;
    if(imp->obj->place().placeX < 0 && imp->obj->place().placeY < 0)
        throw std::runtime_error ("Object not found");
    std::cout << "Start position object with ID" << imp->obj->objectID() << " : [" << imp->obj->place().placeX << ", " << imp->obj->place().placeY << "]" << std::endl;
    if(!imp->obj->getPosition(imp->obj, dt))
        throw std::runtime_error ("Unknown position");
    else
    {
        cmd_search->execute(imp->p_map_a, imp->obj);
        cmd_search->execute(imp->p_map_b, imp->obj);
    }
    std::cout << "Finish position object with ID" << imp->obj->objectID() << " : [" << imp->obj->place().placeX << ", " << imp->obj->place().placeY << "]" << std::endl;
}

int MoveCommand::get_Id_cmd()
{
    return CommandMove;
}

int MoveCommand::get_Id_parent()
{
    return imp->obj->playerID();
}

object* MoveCommand::obj() const
{
    return imp->obj;
}

std::map<int, system_okr>* MoveCommand::p_map_a() const
{
    return imp->p_map_a;
}

std::map<int, system_okr>* MoveCommand::p_map_b() const
{
    return imp->p_map_b;
}

void CheckPositionCommand::execute(std::map<int, system_okr>* p_map, object* obj)
{
    bool change_place = false;
    size_t count_map_chize = p_map->size();
    for(int i = 0; i < count_map_chize; i++)
    {
        if(!p_map->at(i).isEmpty)
        {
            if(isnXY(p_map, obj, i))
            {
                for(std::vector<int>::iterator it = p_map->at(i).obj_id.begin();
                                               it != p_map->at(i).obj_id.end();)
                {
                    if(*it == obj->objectID())
                    {
                        p_map->at(i).countObject--;
                        if(p_map->at(i).countObject == 0)
                            p_map->at(i).isEmpty = true;
std::cout << "object id:" << *it << " delete from " << p_map->at(i).id_syst_okr << " in p_map:size " << p_map->size() << std::endl;
                        it = p_map->at(i).obj_id.erase(it);
                        change_place = true;
                    }
                    else
                        ++it;
                }
            }
        }
        if(change_place)
        {
            size_t count1 = p_map->size();
            for(int j = 0; j < count1; ++j)
            {
                if(isXY(p_map, obj, j))
                {
                    p_map->at(j).countObject++;
                    if(p_map->at(i).isEmpty == true)
                        p_map->at(j).isEmpty = false;
                    int id_obj = obj->objectID();
                    p_map->at(j).obj_id.push_back(id_obj);
std::cout << "object id:" << p_map->at(j).obj_id.back() << " rewrite in " << p_map->at(j).id_syst_okr << " in p_map:size " << p_map->size() << std::endl;
                    change_place = false;
                    break;
                }
            }
        }
    }
}

class RotateCommandP
{
public:
    object* obj;

    RotateCommandP(object* obj) :
        obj(obj)
    {
    }
};

RotateCommand::RotateCommand(object* obj) :
    imp(new RotateCommandP(obj))
{}

RotateCommand::~RotateCommand() { delete imp;}

void RotateCommand::execute()
{
    if(imp->obj->place().placeX < 0 && imp->obj->place().placeY < 0)
        throw std::runtime_error ("Object not found");
    if(!imp->obj->getAngular(imp->obj, 0))
        throw std::runtime_error ("Unknown velocity");
    std::cout << "Start angular " << imp->obj->place().angular << std::endl;
    if(!imp->obj->getAngular(imp->obj, dt))
        throw std::runtime_error ("Unknown position");
    std::cout << "Finish angular " << imp->obj->place().angular << std::endl;
}

int RotateCommand::get_Id_cmd()
{
    return CommandRotate;
}

int RotateCommand::get_Id_parent()
{
    return imp->obj->playerID();
}

object* RotateCommand::obj() const
{
    return imp->obj;
}

class StartMotionP
{
public:
    object* obj;
    double du;

    StartMotionP(object* obj, double du) :
        obj(obj), du(du)
    {
    }
};

StartMotion::StartMotion(object* obj, double du) :
    imp(new StartMotionP(obj, du))
{}

StartMotion::~StartMotion() { delete imp;}

void StartMotion::execute()
{
    if(imp->obj->place().placeX < 0 && imp->obj->place().placeY < 0)
        throw std::runtime_error ("Object not found");
    imp->obj->getVelocity(imp->obj, imp->du);
    std::cout << "Object with id [" << imp->obj->objectID() << "] is start motion" << std::endl;
}

int StartMotion::get_Id_cmd()
{
    return CommandStart;
}

int StartMotion::get_Id_parent()
{
    return imp->obj->playerID();
}

object* StartMotion::obj() const
{
    return imp->obj;
}

class StopMotionP
{
public:
    object* obj;

    StopMotionP(object* obj) :
        obj(obj)
    {
    }
};

StopMotion::StopMotion(object* obj) :
    imp(new StopMotionP(obj))
{}

StopMotion::~StopMotion() { delete imp;}

void StopMotion::execute()
{
    if(imp->obj->place().placeX < 0 && imp->obj->place().placeY < 0)
        throw std::runtime_error ("Object not found");
    std::cout << "Object with id [" << imp->obj->objectID() << "] have velocity before StopMotion = "
              << imp->obj->state().velocity << std::endl;
    imp->obj->getVelocity(imp->obj, -(imp->obj->state().velocity));
    std::cout << "Object with id [" << imp->obj->objectID() << "] have velocity after StopMotion = "
              << imp->obj->state().velocity << std::endl;
}

int StopMotion::get_Id_cmd()
{
    return CommandStop;
}

int StopMotion::get_Id_parent()
{
    return imp->obj->playerID();
}

object* StopMotion::obj() const
{
    return imp->obj;
}

class ShootCommandP
{
public:
    objectVector* vect;
    object* obj;
    std::map<int, system_okr>* p_map_a;
    std::map<int, system_okr>* p_map_b;
    double speed;
    double angle;

    ShootCommandP(std::map<int, system_okr>* p_map_a,
                  std::map<int, system_okr>* p_map_b,
                  objectVector* vect, object* obj,
                  double speed, double angle) :
        p_map_a(p_map_a), p_map_b(p_map_b), vect(vect), obj(obj), speed(speed), angle(angle)
    {
    }
};

ShootCommand::ShootCommand(std::map<int, system_okr>* p_map_a,
                           std::map<int, system_okr>* p_map_b,
                           objectVector* vect, object* obj,
                           double speed, double angle) :
    imp(new ShootCommandP(p_map_a, p_map_b, vect, obj, speed, angle))
{}

ShootCommand::~ShootCommand() { delete imp;}

void ShootCommand::execute()
{
    int playerID = imp->obj->playerID();

    std::list<int> pl_obj;
    int i1 = imp->vect->count();
    for(int i2 = 0; i2 < i1; i2++)
    {
        if(imp->vect->at(i2)->playerID() == playerID)
            pl_obj.push_back(imp->vect->at(i2)->objectID());
    }
    int objectId = pl_obj.back();
    if(objectId < 100)
        objectId = playerID * 100 + 1;
    else
        objectId++;

    int i = imp->vect->count();
    coord place;
    react state;

    place.placeX = imp->obj->place().placeX;
    place.placeY = imp->obj->place().placeY;
    place.angular = imp->angle;

    if(imp->speed > 0.)
        state.velocity = imp->speed + Udelta;
    else if(imp->speed < 0.)
        state.velocity = imp->speed - Udelta;
    state.angularVelocity = 0;
    state.fuel = 9999;

    imp->vect->add(playerID, objectId, state, place);
std::cout << "Create new object (shoot) "   << imp->vect->at(i)->objectID()
          << ": u = "                       << imp->vect->at(i)->state().velocity
          << ", v = "                       << imp->vect->at(i)->state().angularVelocity
          << ", f = "                       << imp->vect->at(i)->state().fuel
          << ", x = "                       << imp->vect->at(i)->place().placeX
          << ", y = "                       << imp->vect->at(i)->place().placeY
          << ", a = "                       << imp->vect->at(i)->place().angular << std::endl;
    //-------------------------------------------------------------
    // помещаем ракеты в системы окресностей
    //-------------------------------------------------------------
    object_position op;
    imp->p_map_a = op.func_obj_r(imp->p_map_a, imp->vect->at(i));
    imp->p_map_b = op.func_obj_r(imp->p_map_b, imp->vect->at(i));
    MoveCommand* cmd_roket = new MoveCommand(imp->p_map_a, imp->p_map_b, imp->vect->at(i));
    cmd_roket->execute();
    BurnCommand* cmd_burn = new BurnCommand(imp->vect->at(i));
    cmd_burn->execute();
}

int ShootCommand::get_Id_cmd()
{
    return CommandStop;
}

int ShootCommand::get_Id_parent()
{
    return imp->obj->playerID();
}

object* ShootCommand::obj() const
{
    return imp->obj;
}

objectVector* ShootCommand::vect() const
{
    return imp->vect;
}

std::map<int, system_okr>* ShootCommand::p_map_a() const
{
    return imp->p_map_a;
}

std::map<int, system_okr>* ShootCommand::p_map_b() const
{
    return imp->p_map_b;
}

class InternetCommandP
{
public:
    objectVector* vect;
    object* obj;
    order *order_cmd;
    std::list<ICommand*> *cmds;
    std::map<int, system_okr>* p_map_a;
    std::map<int, system_okr>* p_map_b;

    InternetCommandP(std::map<int, system_okr>* p_map_a,
                     std::map<int, system_okr>* p_map_b,
                     objectVector* vect, object* obj, order *order_cmd,
                     std::list<ICommand*> *cmds) :
        p_map_a(p_map_a),
        p_map_b(p_map_b),
        vect(vect),
        obj(obj),
        order_cmd(order_cmd),
        cmds(cmds)
    {
    }
};

InternetCommand::InternetCommand(std::map<int, system_okr>* p_map_a,
                                 std::map<int, system_okr>* p_map_b,
                                 objectVector* vect, object* obj,
                                 order *order_cmd, std::list<ICommand *> *cmds) :
    imp(new InternetCommandP(p_map_a, p_map_b, vect, obj, order_cmd, cmds))
{
    create();
    for(int i = 0; i < imp->cmds->size(); i++)
    {
        cmds->push_back(imp->cmds->front());
        imp->cmds->pop_front();
    }
}

InternetCommand::~InternetCommand() { delete imp;}

bool InternetCommand::create()
{
    int IDplayer_message = imp->order_cmd->playerID();
    int IDplayer_object  = imp->obj->playerID();
    bool isEqival = IDplayer_message == IDplayer_object;
    if(isEqival)
    {
        if(imp->order_cmd->actionName() == "StartMove")
        {
            StartMotion *cmd_start = new StartMotion(imp->obj, imp->obj->state().velocity);
            imp->cmds->push_back(cmd_start);
        }
        else if(imp->order_cmd->actionName() == "StopMove")
        {
            StopMotion *cmd_stop = new StopMotion(imp->obj);
            imp->cmds->push_back(cmd_stop);
        }
        else if(imp->order_cmd->actionName() == "Shoot")
        {
            ShootCommand *cmd_shoot = new ShootCommand(imp->p_map_a, imp->p_map_b, imp->vect, imp->obj, Umax, 0.);
            imp->cmds->push_back(cmd_shoot);
        }
        return true;
    }
    else
    {
        std::cout << " Command will be ignored! " << std::endl;
        return false;
    }
}

void InternetCommand::execute()
{
    if(imp->cmds->empty())
        throw std::runtime_error ("Сommand list is empty");
    for(int i = 0; i < imp->cmds->size(); i++)
    {
        try
        {
            imp->cmds->front()->execute();
        } catch (...) {
            throw std::runtime_error ("Pass the error up");
        }
        imp->cmds->pop_front();
    }
}

int InternetCommand::get_Id_parent()
{
    return imp->obj->playerID();
}

std::map<int, system_okr>* InternetCommand::p_map_a() const
{
    return imp->p_map_a;
}

std::map<int, system_okr>* InternetCommand::p_map_b() const
{
    return imp->p_map_b;
}

class BurnCommandP
{
public:
    object* obj;

    BurnCommandP(object* obj) :
        obj(obj)
    {
    }
};

BurnCommand::BurnCommand(object* obj) :
    imp(new BurnCommandP(obj))
{}

BurnCommand::~BurnCommand() { delete imp;}

void BurnCommand::execute()
{
    if(imp->obj->place().placeX < 0 && imp->obj->place().placeY < 0)
        throw std::runtime_error ("Object not found");
    imp->obj->getFuel(imp->obj, 1);
}

int BurnCommand::get_Id_cmd()
{
    return CommandBurn;
}

int BurnCommand::get_Id_parent()
{
    return imp->obj->playerID();
}

object* BurnCommand::obj() const
{
    return imp->obj;
}

class CheckCommandP
{
public:
    object* obj;

    CheckCommandP(object* obj) :
        obj(obj)
    {
    }
};

CheckCommand::CheckCommand(object* obj) :
    imp(new CheckCommandP(obj))
{}

CheckCommand::~CheckCommand() { delete imp;}

void CheckCommand::execute()
{
    if(imp->obj->place().placeX < 0 && imp->obj->place().placeY < 0)
        throw std::runtime_error ("Object not found");
}

int CheckCommand::get_Id_cmd()
{
    return CommandCheck;
}

int CheckCommand::get_Id_parent()
{
    return imp->obj->playerID();
}

object* CheckCommand::obj() const
{
    return imp->obj;
}
