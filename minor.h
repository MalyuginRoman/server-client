#ifndef MINOR_H
#define MINOR_H
#include <vector>
#include <list>

const int dt = 1;
const int dF = 1;
const double eps = 10E-6;
const double Xmax = 100.;
const double Ymax = 100.;
const int count_system_1 = 4;
const int count_system_2 = 5;
const double Umax = 15.;
const double Udelta = 5.;

/*!
  Координаты и ориентация объекта
*/
struct coord
{
    double placeX;
    double placeY;
    double angular;
};

/*!
  Линейная и угловая скорости объекта, запас топлива
*/
struct react
{
    double velocity;
    double angularVelocity;
    double fuel;
};

/*!
  ID номер действия и временной шаг, на котором это действие происходит
*/
class action
{
public:
    action(int actionID, int timeStep);
    ~action();

    int actionID() const;
    int timeStep() const;

private:
    class actionP* imp;
};

struct coord_interval
{
public:
    double Xmin;
    double Xmax;
    double Ymin;
    double Ymax;
};

struct system_okr
{
public:
    int id_syst_okr;
    bool isEmpty;
    int countObject;
    coord_interval XY;
    std::vector<int> obj_id;
};

struct objectCheck
{
public:
    system_okr p_map;
};

struct collisionObjects
{
public:
    bool isActive;
    int obj1;
    int obj2;
};

struct game_confige
{
public:
    int game_status;
    int number_player;
    std::list<std::string> player_name;
};

struct game
{
    int gameID;
    int game_status;
    int connection_player;
    std::vector<std::string> player_name;
};

struct go_game
{
    int game_status;
    std::vector<std::string> player_name;
};

#endif // MINOR_H
