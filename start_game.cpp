#include <string>
#include "start_game.h"
#include "objposition.h"
#include "scope.h"

class gameP
{
public:
    std::map<std::string, std::function<ICommand*()>> *m_map;
    std::map<std::string, std::string> *m_scope;

    gameP(std::map<std::string, std::function<ICommand*()>> *m_map,
                std::map<std::string, std::string> *m_scope) :
        m_map(m_map),
        m_scope(m_scope)
    {
    }
};

start_game::start_game(std::map<std::string, std::function<ICommand*()>> *m_map,
                       std::map<std::string, std::string> *m_scope) :
    imp(new gameP(m_map, m_scope))
{
}

start_game::~start_game() { delete imp;}

void start_game::create_game()
{
// ...
//-------------------------------------------------------------
// формируем системы окрестностей
//-------------------------------------------------------------
    object_position op;
    std::map<int, system_okr> p_map_c_a;
    std::map<int, system_okr> p_map_c_b;
    p_map_c_a = op.func_name(1);
    p_map_c_b = op.func_name(2);
//-------------------------------------------------------------
//   Создаем объекты
//-------------------------------------------------------------
    objectVector vector;
    int count = 2;
    for(int i = 0; i < count; i++)
    {
        int playerID = i + 1;
        int objectID = playerID * 10 + 1;
        coord place;
        react state;

        place.placeX = 1. + 20. * i;
        if(place.placeX > 100.)
            place.placeX = 100. - (place.placeX - 100.);
        place.placeY = 1. + 25. * i;
        if(place.placeY > 100.)
            place.placeY = 100. - (place.placeY - 100.);
        place.angular = 45 * (i + 1);

        state.velocity = 40;
        state.angularVelocity = 20;
        state.fuel = 10;

        vector.add(playerID, objectID, state, place);
    }

    for(int i = 0; i < count; i++)
    {
        std::cout << vector.at(i)->playerID() << ", " << vector.at(i)->objectID()
                                          << ": u = " << vector.at(i)->state().velocity
                                          << ", v = " << vector.at(i)->state().angularVelocity
                                          << ", f = " << vector.at(i)->state().fuel
                                          << ", x = " << vector.at(i)->place().placeX
                                          << ", y = " << vector.at(i)->place().placeY
                                          << ", a = " << vector.at(i)->place().angular << std::endl;
    }
//-------------------------------------------------------------
// помещаем объекты в системы окресностей
//-------------------------------------------------------------
    p_map_c_a = op.func_obj(p_map_c_a, &vector);
    p_map_c_b = op.func_obj(p_map_c_b, &vector);
//-------------------------------------------------------------
//   Общие переменные
//-------------------------------------------------------------
    //std::map<std::string, std::function<ICommand*()>> m_map;
    //std::map<std::string, std::string> m_scope;
    scope *my_scope = new scope(&m_map, &m_scope);
//-------------------------------------------------------------
//   Регистрация зависимостей в Скоупе (без учета топлива)
//-------------------------------------------------------------
    my_scope = my_scope->fillin(my_scope, p_map_c_a, p_map_c_b, &vector);
//-------------------------------------------------------------
//   Регистрация зависимостей в Скоупе (с учетом топлива)
//-------------------------------------------------------------
    // формируем макрокоманды
    CheckCommand *cmd_check = new CheckCommand();
    BurnCommand *cmd_burn = new BurnCommand();
    MoveCommand *cmd_move = new MoveCommand(&p_map_c_a, &p_map_c_b, vector.at(0));
    RotateCommand *cmd_rotate = new RotateCommand(vector.at(0));
    LogerCommand *cmd_loger = new LogerCommand();
    EmptyCommand *cmd_empty = new EmptyCommand();
    HardStopCommand *cmd_hard = new HardStopCommand();
    SoftStopCommand *cmd_soft = new SoftStopCommand();
    StartMotion *cmd_start = new StartMotion(vector.at(0), vector.at(0)->state().velocity);
    StopMotion *cmd_stop = new StopMotion(vector.at(0));
    ShootCommand *cmd_shoot = new ShootCommand(&vector, vector.at(0));
//-------------------------------------------------------------
    std::list<ICommand*> cmd_list1, cmd_list2;
    cmd_list1.push_back(cmd_check);
    cmd_list1.push_back(cmd_move);
    cmd_list1.push_back(cmd_burn);

    my_scope->resolve("Scope2",
                    "MacroCommand1",
                    [&cmd_list1]() { return new MacroCommand(cmd_list1); });

    cmd_list2.push_back(cmd_check);
    cmd_list2.push_back(cmd_rotate);
    cmd_list2.push_back(cmd_burn);
    my_scope->resolve("Scope2",
                    "MacroCommand2",
                    [&cmd_list2]() { return new MacroCommand(cmd_list2); });
//-------------------------------------------------------------
}
