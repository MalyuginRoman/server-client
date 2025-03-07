#include <iostream>
#include "icommand.h"
#include "object.h"

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
    return (!isX && !isY);
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
    std::cout << "CommandMove" << std::endl;
    if(imp->obj->place().placeX < 0 && imp->obj->place().placeY < 0)
        throw std::runtime_error ("Object not found");
    if(!imp->obj->getVelocity(imp->obj, 0.))
        throw std::runtime_error ("Unknown velocity");
    std::cout << "Start position  [" << imp->obj->place().placeX << ", " << imp->obj->place().placeY << "]" << std::endl;
    if(!imp->obj->getPosition(imp->obj, dt))
        throw std::runtime_error ("Unknown position");
    else
    {
        cmd_search->execute(imp->p_map_a, imp->obj);
        cmd_search->execute(imp->p_map_b, imp->obj);
    }
    std::cout << "Finish position [" << imp->obj->place().placeX << ", " << imp->obj->place().placeY << "]" << std::endl;
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
    std::cout << "I am in CheckPositionCommand!" << std::endl;
    std::cout << "Finish position [" << obj->place().placeX << ", " << obj->place().placeY << "]" << std::endl;

    bool change_place = false;
    size_t count_map_chize = p_map->size();
    for(int i = 0; i < count_map_chize; i++)
    {
        if(!p_map->at(i).isEmpty)
        {
            if(isnXY(p_map, obj, i))
            {
                p_map->at(i).countObject--;
                if(p_map->at(i).countObject == 0)
                    p_map->at(i).isEmpty = true;
std::cout << "object id:" << p_map->at(i).obj_id.front() << " delete from " << p_map->at(i).id_syst_okr << " in p_map:size " << p_map->size() << std::endl;
                p_map->at(i).obj_id.pop_back();
                change_place = true;
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
    std::cout << "CommandRotate" << std::endl;
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
    std::cout << "StartMotion" << std::endl;
    if(imp->obj->place().placeX < 0 && imp->obj->place().placeY < 0)
        throw std::runtime_error ("Object not found");
    imp->obj->getVelocity(imp->obj, 0.);
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
    std::cout << "StopMotion" << std::endl;
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

    ShootCommandP(objectVector* vect, object* obj) :
        vect(vect), obj(obj)
    {
    }
};

ShootCommand::ShootCommand(objectVector* vect, object* obj) :
    imp(new ShootCommandP(vect, obj))
{}

ShootCommand::~ShootCommand() { delete imp;}

void ShootCommand::execute()
{
    std::cout << "ShootCommand" << std::endl;
// create new object (torpeda)
    int playerID = imp->obj->playerID();
    int objectId = playerID * 100 + 1;
    int i = imp->vect->count();
    coord place;
    react state;

    place.placeX = imp->obj->place().placeX;
    place.placeY = imp->obj->place().placeY;
    place.angular = imp->obj->place().angular;

    state.velocity = Umax;
    state.angularVelocity = 0;
    state.fuel = -9999;

    imp->vect->add(playerID, objectId, state, place);
std::cout << "Create new object (shoot) "   << imp->vect->at(i)->objectID()
          << ": u = "                       << imp->vect->at(i)->state().velocity
          << ", v = "                       << imp->vect->at(i)->state().angularVelocity
          << ", f = "                       << imp->vect->at(i)->state().fuel
          << ", x = "                       << imp->vect->at(i)->place().placeX
          << ", y = "                       << imp->vect->at(i)->place().placeY
          << ", a = "                       << imp->vect->at(i)->place().angular << std::endl;
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

class InternetCommandP
{
public:
    objectVector* vect;
    object* obj;
    order *order_cmd;
    std::list<ICommand*> *cmds;

    InternetCommandP(objectVector* vect, object* obj, order *order_cmd,
                     std::list<ICommand*> *cmds) :
        vect(vect),
        obj(obj),
        order_cmd(order_cmd),
        cmds(cmds)
    {
    }
};

InternetCommand::InternetCommand(objectVector* vect, object* obj, order *order_cmd, std::list<ICommand *> *cmds) :
    imp(new InternetCommandP(vect, obj, order_cmd, cmds))
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
            ShootCommand *cmd_shoot = new ShootCommand(imp->vect, imp->obj);
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
    std::cout << "InternetCommand" << std::endl;
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
