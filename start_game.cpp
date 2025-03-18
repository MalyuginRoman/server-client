#include <string>
#include "start_game.h"
#include "objposition.h"
#include "scope.h"
#include "safequeue.h"
#include "eventloop.h"

class gameP
{
public:
    std::map<std::string, std::function<ICommand*()>> m_map;
    std::map<std::string, std::string> m_scope;
    std::vector<player> *player_list;
    objectVector *obj_vector;


    gameP(std::map<std::string, std::function<ICommand*()>> m_map,
                std::map<std::string, std::string> m_scope,
                std::vector<player> *player_list,
                objectVector *obj_vector) :
        m_map(m_map),
        m_scope(m_scope),
        player_list(player_list),
        obj_vector(obj_vector)
    {
    }
};

start_game::start_game(std::map<std::string, std::function<ICommand*()>> m_map,
                       std::map<std::string, std::string> m_scope,
                       std::vector<player> *player_list,
                       objectVector *obj_vector) :
    imp(new gameP(m_map, m_scope, player_list, obj_vector))
{
}

start_game::~start_game() { delete imp;}

std::map<int, system_okr> start_game::create_pole(int type)
{
//-------------------------------------------------------------
// формируем системы окрестностей
//-------------------------------------------------------------
    object_position op;
    return op.func_name(type);
//-------------------------------------------------------------
}

void start_game::create_player(std::string playerName,
                               std::vector<player> *player_list,
                               objectVector *obj_vector,
                               std::map<int, system_okr> *p_map_c_a,
                               std::map<int, system_okr> *p_map_c_b)
{
//-------------------------------------------------------------
//   Создаем объекты
//-------------------------------------------------------------
    player currentPlayer;
    currentPlayer.playerName = playerName;
    if(player_list->empty())
        currentPlayer.playerId = 1;
    else
        currentPlayer.playerId = player_list->size() + 1;

    currentPlayer.playerObject.push_back(currentPlayer.playerId * 10 + 1);
    coord place;
    react state;

    int playerID = currentPlayer.playerId;
    int objectID = currentPlayer.playerObject.at(0);
    if(playerID == 1)
    {
        place.placeX = 0.;
        place.placeY = 50.;
        place.angular = 0;
        state.velocity = 0;
    }
    else if(playerID == 2)
    {
        place.placeX = 30.;
        place.placeY = 50.;
        place.angular = 0;
        state.velocity = 0;
    }
    else if(playerID == 3)
    {
        place.placeX = 50.;
        place.placeY = 100.;
        place.angular = 270;
        state.velocity = 0;
    }
    else if(playerID == 4)
    {
        place.placeX = 50.;
        place.placeY = 0.;
        place.angular = 90;
        state.velocity = 0;
    }
    else if(playerID == 5)
    {
        place.placeX = 50.;
        place.placeY = 50.;
        place.angular = 0;
        state.velocity = 0;
    }

    state.angularVelocity = 0;
    state.fuel = 100;

    obj_vector->add(playerID, objectID, state, place);
    player_list->push_back(currentPlayer);

    object_position op;
//-------------------------------------------------------------
// помещаем объекты в системы окресностей
//-------------------------------------------------------------
    p_map_c_a = op.func_obj(p_map_c_a, obj_vector);
    p_map_c_b = op.func_obj(p_map_c_b, obj_vector);
}



game_scope start_game::filling_scope(std::map<std::string, std::function<ICommand*()>> m_map,
                                     std::map<std::string, std::string> m_scope)
{
    game_scope new_scope;
//-------------------------------------------------------------
//   Регистрация пустой дефолтной зависимости в Скоупе
//-------------------------------------------------------------
    RegisterCommand *cmd_registr = new RegisterCommand(&m_map, &m_scope);
    cmd_registr->registerType(
                "Scope1",
                "None",
                []() {
                return new EmptyCommand(); });
    new_scope.m_map = m_map;
    new_scope.m_scope = m_scope;
    return new_scope;
}


void start_game::play_round(orderVector *player_messages,
                            std::vector<player> *player_list,  // для добавления ракет при выстреле
                            objectVector *obj_vector,
                            std::map<int, system_okr> *p_map_c_a,
                            std::map<int, system_okr> *p_map_c_b,
                            std::map<std::string, std::function<ICommand*()>> m_map,
                            std::map<std::string, std::string> m_scope)
{
    EmptyCommand *cmd_empty = new EmptyCommand();
    StateStatus *sc = new StateStatus(new DefaultState(), cmd_empty);
    SafeQueue<ICommand*> queueCmds;
    eventloop* game = new eventloop(&queueCmds, sc);
    IocContainer<ICommand> ioc;

    std::string player_command;
    for(order* i : player_messages->vector())
    {
        player_command = i->actionName();
        if(player_command == "StartMove")
        {
            int object_count = obj_vector->count();
            for(int j = 0; j < object_count; j++)
            {
                int obj_com = i->objectID();
                int obj_cur = obj_vector->at(j)->objectID();
                if(obj_com == obj_cur)
                {
                    RegisterCommand *cmd_registr = new RegisterCommand(&m_map, &m_scope);
                    cmd_registr->registerType(
                                "Scope1",
                                "StartMove",
                                [&obj_vector, &p_map_c_a, &p_map_c_b, &i, &j]() {
                                int newVelocity = stoi(i->specParam());
                                obj_vector->at(j)->getVelocity(obj_vector->at(j), newVelocity);
                                CheckCommand *cmd_check = new CheckCommand(obj_vector->at(j));
                                MoveCommand *cmd_move = new MoveCommand(p_map_c_a, p_map_c_b, obj_vector->at(j));
                                BurnCommand *cmd_burn = new BurnCommand(obj_vector->at(j));
                                std::list<ICommand*> cmd_list;
                                cmd_list.push_back(cmd_check);
                                cmd_list.push_back(cmd_move);
                                cmd_list.push_back(cmd_burn);
                                return new MacroCommand(cmd_list);});
                    ioc.resolve("StartMove", m_map, m_scope, obj_vector->at(j))->execute();
                }
            }
        }
        else if(player_command == "Go")
        {
            int object_count = obj_vector->count();
            for(int j = 0; j < object_count; j++)
            {
                int obj_com = i->objectID();
                int obj_cur = obj_vector->at(j)->objectID();
                if(obj_com == obj_cur)
                {
                    RegisterCommand *cmd_registr = new RegisterCommand(&m_map, &m_scope);
                    cmd_registr->registerType(
                                "Scope1",
                                "Go",
                                [&obj_vector, &p_map_c_a, &p_map_c_b, &j]() {
                                CheckCommand *cmd_check = new CheckCommand(obj_vector->at(j));
                                MoveCommand *cmd_move = new MoveCommand(p_map_c_a, p_map_c_b, obj_vector->at(j));
                                BurnCommand *cmd_burn = new BurnCommand(obj_vector->at(j));
                                std::list<ICommand*> cmd_list;
                                cmd_list.push_back(cmd_check);
                                cmd_list.push_back(cmd_move);
                                cmd_list.push_back(cmd_burn);
                                return new MacroCommand(cmd_list);});
                    ioc.resolve("Go", m_map, m_scope, obj_vector->at(j))->execute();
                }
            }
        }
        else if(player_command == "Rotate")
        {
            int object_count = obj_vector->count();
            for(int j = 0; j < object_count; j++)
            {
                int obj_com = i->objectID();
                int obj_cur = obj_vector->at(j)->objectID();
                if(obj_com == obj_cur)
                {
                    RegisterCommand *cmd_registr = new RegisterCommand(&m_map, &m_scope);
                    cmd_registr->registerType(
                                "Scope1",
                                "Rotate",
                                [&obj_vector, &p_map_c_a, &p_map_c_b, &i, &j]() {
                                int newAngle = stoi(i->specParam());
                                obj_vector->at(j)->getAngularVelocity(obj_vector->at(j), newAngle);
                                CheckCommand *cmd_check = new CheckCommand(obj_vector->at(j));
                                RotateCommand *cmd_rotate = new RotateCommand(obj_vector->at(j));
                                MoveCommand *cmd_move = new MoveCommand(p_map_c_a, p_map_c_b, obj_vector->at(j));
                                BurnCommand *cmd_burn = new BurnCommand(obj_vector->at(j));
                                std::list<ICommand*> cmd_list;
                                cmd_list.push_back(cmd_check);
                                cmd_list.push_back(cmd_rotate);
                                cmd_list.push_back(cmd_move);
                                cmd_list.push_back(cmd_burn);
                                return new MacroCommand(cmd_list);});
                    ioc.resolve("Rotate", m_map, m_scope, obj_vector->at(j))->execute();
                }
            }
        }
        else if(player_command == "StopMove")
        {
            int object_count = obj_vector->count();
            for(int j = 0; j < object_count; j++)
            {
                int obj_com = i->objectID();
                int obj_cur = obj_vector->at(j)->objectID();
                if(obj_com == obj_cur)
                {
                    RegisterCommand *cmd_registr = new RegisterCommand(&m_map, &m_scope);
                    cmd_registr->registerType(
                                "Scope1",
                                "StopMove",
                                [&obj_vector, &p_map_c_a, &p_map_c_b, &j]() {
                                int newVelocity = 0;
                                obj_vector->at(j)->getVelocity(obj_vector->at(j), newVelocity);
                                return new MoveCommand(p_map_c_a, p_map_c_b, obj_vector->at(j)); });
                    ioc.resolve("StopMove", m_map, m_scope, obj_vector->at(j))->execute();
                }
            }
        }
        else if(player_command == "Shoot")
        {
            int object_count = obj_vector->count();
            for(int j = 0; j < object_count; j++)
            {
                int current_player;
                int player_count = player_list->size();
                for(int k = 0; k < player_count; k++)
                {
                    if(player_list->at(k).playerId == i->playerID())
                        current_player = k;
                }
                int i1 = player_list->at(current_player).playerObject.size() - 1;
                int objectId = player_list->at(current_player).playerObject.at(i1);
                if(objectId < 100)
                    objectId = i->playerID() * 100 + 1;
                else
                    objectId++;

                int obj_com = i->objectID();
                int obj_cur = obj_vector->at(j)->objectID();
                if((obj_com == obj_cur) && (obj_vector->at(j)->state().velocity != 0.))
                {
                    RegisterCommand *cmd_registr = new RegisterCommand(&m_map, &m_scope);
                    cmd_registr->registerType(
                                "Scope1",
                                "Shoot",
                                [&obj_vector, &player_list, &p_map_c_a, &p_map_c_b, &j,
                                &current_player, &objectId]() {
                                CheckCommand *cmd_check = new CheckCommand(obj_vector->at(j));
                                ShootCommand *cmd_shoot = new ShootCommand(p_map_c_a, p_map_c_b, obj_vector, obj_vector->at(j),
                                                                           obj_vector->at(j)->state().velocity,
                                                                           obj_vector->at(j)->place().angular);
                                MoveCommand *cmd_move = new MoveCommand(p_map_c_a, p_map_c_b, obj_vector->at(j));
                                BurnCommand *cmd_burn = new BurnCommand(obj_vector->at(j));
                                std::list<ICommand*> cmd_list;
                                cmd_list.push_back(cmd_check);
                                cmd_list.push_back(cmd_shoot);
                                cmd_list.push_back(cmd_move);
                                cmd_list.push_back(cmd_burn);
                                player_list->at(current_player).playerObject.push_back(objectId);
                                return new MacroCommand(cmd_list);});
                    ioc.resolve("Shoot", m_map, m_scope, obj_vector->at(j))->execute();
                }
                else if ((obj_com == obj_cur) && obj_vector->at(j)->state().velocity == 0.)
                {
                    std::cout << "This object cann't shooting! Need be in motion!" << std::endl;
                    queueCmds.push(cmd_empty);
                }
            }
        }
        else if(player_command == "None")
        {
            int object_count = obj_vector->count();
            for(int j = 0; j < object_count; j++)
            {
                int obj_com = i->objectID();
                int obj_cur = obj_vector->at(j)->objectID();
                if(obj_com == obj_cur)
                {
                    ioc.resolve("None", m_map, m_scope, obj_vector->at(j))->execute();
                }
            }
        }
        queueCmds.push(cmd_empty);
//------------------------------------------------------------------- // проверяем имеются ли у игрока запущенные ракеты
        std::list<int> pl_obj;
        int k1 = imp->obj_vector->count();
        for(int k2 = 0; k2 < k1; k2++)
        {
            if(imp->obj_vector->at(k2)->playerID() == i->playerID())
                pl_obj.push_back(imp->obj_vector->at(k2)->objectID());
        }
        if(pl_obj.size() > 1)
        {
            int object_count = obj_vector->count();
            for(int j = 0; j < object_count; j++)
            {
                int obj_com = i->playerID();
                int obj_cur = obj_vector->at(j)->playerID();
                if((obj_com == obj_cur) && (obj_vector->at(j)->objectID() > 100))
                {
                    CheckCommand *cmd_check = new CheckCommand(obj_vector->at(j));
                    MoveCommand *cmd_move = new MoveCommand(p_map_c_a, p_map_c_b, obj_vector->at(j));
                    BurnCommand *cmd_burn = new BurnCommand(obj_vector->at(j));
                    queueCmds.push(cmd_check);
                    queueCmds.push(cmd_move);
                    queueCmds.push(cmd_burn);
                }
            }
        }
//-------------------------------------------------------------------
    }
    game->start(&queueCmds, sc);
}

collisionObjects start_game::check_collision(objectVector *obj_vector,
                                 std::map<int, system_okr> *p_map_c_a,
                                 std::map<int, system_okr> *p_map_c_b)
{
    collisionObjects result;
    result.isActive = false;
    bool isDel = false;
    int a1 = p_map_c_a->size();
    for(int i = 0; i < a1; i++)
    {
        if(!p_map_c_a->at(i).isEmpty)
        {
            int object_count = p_map_c_a->at(i).countObject;
            if(object_count > 1)
            {
                for(int k = 0; k < (object_count - 1); k++)
                {
                    double X1 = obj_vector->at(k    )->place().placeX;
                    double X2 = obj_vector->at(k + 1)->place().placeX;
                    double Y1 = obj_vector->at(k    )->place().placeY;
                    double Y2 = obj_vector->at(k + 1)->place().placeY;
    std::cout << obj_vector->at(k)->objectID() << " : " << obj_vector->at(k + 1)->objectID();
    std::cout << " => " << X1 << " : " << X2 << " ; " << Y1 << " : " << Y2 << std::endl;
                    if((abs(X1 - X2) < eps) && (abs(Y1 - Y2) < eps))
                    {
                        result.isActive = true;
                        result.obj1 = obj_vector->at(k)->objectID();
                        result.obj2 = obj_vector->at(k + 1)->objectID();
                        obj_vector->del(k + 1);
                        obj_vector->del(k);
                        isDel = true;
                    }
                }
                if(!isDel)
                {
                    double X1 = obj_vector->at(0)->place().placeX;
                    double X2 = obj_vector->at(object_count - 1)->place().placeX;
                    double Y1 = obj_vector->at(0)->place().placeY;
                    double Y2 = obj_vector->at(object_count - 1)->place().placeY;
    std::cout << obj_vector->at(0)->objectID() << " : " << obj_vector->at(object_count - 1)->objectID();
    std::cout << " => " << X1 << " : " << X2 << " ; " << Y1 << " : " << Y2 << std::endl;
                    if((abs(X1 - X2) < eps) && (abs(Y1 - Y2) < eps))
                    {
                        result.isActive = true;
                        result.obj1 = obj_vector->at(0)->objectID();
                        result.obj2 = obj_vector->at(object_count - 1)->objectID();
                        obj_vector->del(object_count - 1);
                        obj_vector->del(0);
                        isDel = true;
                    }
                }
            }
            if(isDel)
            {
                for(std::vector<int>::iterator it = p_map_c_a->at(i).obj_id.begin();
                                               it != p_map_c_a->at(i).obj_id.end();)
                {
                    if((*it == result.obj1) || (*it == result.obj2))
                    {
                        p_map_c_a->at(i).countObject--;
                        if(p_map_c_a->at(i).countObject == 0)
                            p_map_c_a->at(i).isEmpty = true;
std::cout << "object id:" << *it << " delete from " << p_map_c_a->at(i).id_syst_okr << " in p_map:size " << p_map_c_a->size() << " by collisiun" << std::endl;
                        it = p_map_c_a->at(i).obj_id.erase(it);
                    }
                    else
                        ++it;
                }
            }
        }
    }
    int a2 = p_map_c_b->size();
    if(!isDel)
    {
        for(int i = 0; i < a2; i++)
        {
            if(!p_map_c_b->at(i).isEmpty)
            {
                int object_count = p_map_c_b->at(i).countObject;
                if(object_count > 2)
                {
                    for(int k = 0; k < (object_count - 1); k++)
                    {
                        double X1 = obj_vector->at(k    )->place().placeX;
                        double X2 = obj_vector->at(k + 1)->place().placeX;
                        double Y1 = obj_vector->at(k    )->place().placeY;
                        double Y2 = obj_vector->at(k + 1)->place().placeY;
        std::cout << X1 << " : " << X2 << " ; " << Y1 << " : " << Y2 << std::endl;
                        if((abs(X1 - X2) < eps) && (abs(Y1 - Y2) < eps))
                        {
                            result.isActive = true;
                            result.obj1 = obj_vector->at(k)->objectID();
                            result.obj2 = obj_vector->at(k + 1)->objectID();
                            obj_vector->del(k + 1);
                            obj_vector->del(k);
                        }
                    }
                }
            }
            if(isDel)
            {
                for(std::vector<int>::iterator it = p_map_c_b->at(i).obj_id.begin();
                                               it != p_map_c_b->at(i).obj_id.end();)
                {
                    if((*it == result.obj1) || (*it == result.obj2))
                    {
                        p_map_c_b->at(i).countObject--;
                        if(p_map_c_b->at(i).countObject == 0)
                            p_map_c_b->at(i).isEmpty = true;
std::cout << "object id:" << *it << " delete from " << p_map_c_b->at(i).id_syst_okr << " in p_map:size " << p_map_c_b->size() << " by collisiun" << std::endl;
                        it = p_map_c_b->at(i).obj_id.erase(it);
                    }
                    else
                        ++it;
                }
            }
        }
    }
    if(!result.isActive)
        std::cout << "Moving without collision" << std::endl;
    else
        std::cout << "Destroy objects " << result.obj1 << " and " << result.obj2 << std::endl;
    return result;
}

void start_game::delete_objects(std::vector<player> *player_list,
                                objectVector *obj_vector)
{
    bool isWork = false;
    std::list<int> del_obj;
    std::list<int> del_obj1, del_obj2;
    int obj_count = obj_vector->count();
    for(int i = 0; i < obj_count; i++)
    {
        double X1 = obj_vector->at(i)->place().placeX;
        double Y1 = obj_vector->at(i)->place().placeY;
        if((X1 > Xmax) || (Y1 > Ymax) || (X1 < 0.) || (Y1 < 0.))
        {
            del_obj.push_back(obj_vector->at(i)->objectID());
            del_obj1.push_back(obj_vector->at(i)->objectID());
        }
    }
    int i1 = player_list->size();
    for (int i = 0; i < i1; i++)
    {
        if(!del_obj.empty())
        {
            for(std::vector<int>::iterator it = player_list->at(i).playerObject.begin();
                                           it!= player_list->at(i).playerObject.end();)
            {
                bool isDelete = false;
                while(!del_obj.empty() && it!= player_list->at(i).playerObject.end())
                {
                    if(*it == del_obj.front())
                    {
                       it = player_list->at(i).playerObject.erase(it);
                       del_obj.pop_front();
                       isDelete = true;
                       isWork = true;
                       break;
                    }
                    else ++it;
                }
                if(!isDelete && it!= player_list->at(i).playerObject.end()) ++it;
            }
        }
    }
    while(!del_obj1.empty())
    {
        std::cout << "Before delete object!!!" << std::endl;
        int obj_vector_count = obj_vector->count();
        for(int i = 0; i < obj_vector_count; i++)
        {
        std::cout << obj_vector->at(i)->playerID()
                  << ", "     << obj_vector->at(i)->objectID()
                  << ": u = " << obj_vector->at(i)->state().velocity
                  << ", v = " << obj_vector->at(i)->state().angularVelocity
                  << ", f = " << obj_vector->at(i)->state().fuel
                  << ", x = " << obj_vector->at(i)->place().placeX
                  << ", y = " << obj_vector->at(i)->place().placeY
                  << ", a = " << obj_vector->at(i)->place().angular;
        if(del_obj1.front() == obj_vector->at(i)->objectID())
        {
            del_obj2.push_back(i);
            del_obj1.pop_front();
            std::cout << " <=======";
        }
        std::cout << std::endl;
        if(del_obj1.empty())
            break;
        }
    }
    while(!del_obj2.empty())
    {
        obj_vector->del(del_obj2.back());
        del_obj2.pop_back();
    }
    int obj_vector_count = obj_vector->count();
    if(isWork)
    {
        std::cout << "After delete object!!!" << std::endl;
        for(int i = 0; i < obj_vector_count; i++)
        {
            std::cout << obj_vector->at(i)->playerID()
                  << ", "     << obj_vector->at(i)->objectID()
                  << ": u = " << obj_vector->at(i)->state().velocity
                  << ", v = " << obj_vector->at(i)->state().angularVelocity
                  << ", f = " << obj_vector->at(i)->state().fuel
                  << ", x = " << obj_vector->at(i)->place().placeX
                  << ", y = " << obj_vector->at(i)->place().placeY
                  << ", a = " << obj_vector->at(i)->place().angular;
            std::cout << std::endl;
        }
    }
}
