#include <QCoreApplication>
#include "game_creator.h"
#include "game_server.h"
#include <thread>

int main(int ac, char **av)
{
//------------------------------- создание игры - подключение
    std::vector<game> games;
    game_creator gc;
    game_server gs;
    gc.default_games(&games);
    while(true)
    {
        SOCKET ServSock = gs.bind_server();
        int gameID = gc.game_create(&games);
        //std::cout << gameID << std::endl;
        bool gs_int = gs.game_server_main(&games, gameID, ServSock);
        //std::cout << gs_int << std::endl;
    }

    return 0;
}
