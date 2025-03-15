#ifndef START_GAME_H
#define START_GAME_H
#include <fstream>
#include <filesystem>
#include <string>
#include <list>
//#include "icommand.h"
#include "objposition.h"
#include "scope.h"

struct player
{
    std::string playerName;
    int playerId;
    std::vector<int> playerObject;
};

class start_game
{
public:
    start_game(std::map<std::string, std::function<ICommand*()>> *m_map,
               std::map<std::string, std::string> *m_scope,
               std::vector<player> *player_list,
               objectVector *obj_vector);
    ~start_game();
    std::map<std::string, std::function<ICommand*()>> m_map;
    std::map<std::string, std::string> m_scope;
    std::vector<player> player_list;
    std::map<int, system_okr> p_map_c_a;
    std::map<int, system_okr> p_map_c_b;
    objectVector obj_vector;
    std::map<int, system_okr> create_pole(int type);
    //scope my_scope;
    void create_player(std::string playerName,
                       std::vector<player> *player_list,
                       objectVector *obj_vector,
                       std::map<int, system_okr> *p_map_c_a,
                       std::map<int, system_okr> *p_map_c_b);
    void play_round(orderVector *player_messages,
                    std::vector<player> *player_list,
                    objectVector *obj_vector,
                    std::map<int, system_okr> *p_map_c_a,
                    std::map<int, system_okr> *p_map_c_b);
    collisionObjects check_collision(objectVector *obj_vector,
                        std::map<int, system_okr> *p_map_c_a,
                        std::map<int, system_okr> *p_map_c_b);
    void delete_objects(std::vector<player> *player_list,
                        objectVector *obj_vector);
private:
    class gameP* imp;
};

#endif // START_GAME_H
