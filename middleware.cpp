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
        //readFuelConfig();
        SOCKET ServSock = gs.bind_server();
        int gameID = gc.game_create(&games);
        bool gs_int = gs.game_server_main(&games, gameID, ServSock);
    }

    return 0;
}
