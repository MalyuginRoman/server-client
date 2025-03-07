#ifndef START_GAME_H
#define START_GAME_H
#include <fstream>
#include <filesystem>
#include <string>
#include "icommand.h"
#include "objposition.h"

class start_game
{
public:
    start_game(std::map<std::string, std::function<ICommand*()>> *m_map,
               std::map<std::string, std::string> *m_scope);
    ~start_game();
    std::map<std::string, std::function<ICommand*()>> m_map;
    std::map<std::string, std::string> m_scope;
    void create_game();
private:
    class gameP* imp;
};

#endif // START_GAME_H
