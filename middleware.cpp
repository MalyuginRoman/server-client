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
    SOCKET ServSock = gs.bind_server();
    gc.default_games(&games);
    bool isOk = gc.game_create(&games);
    std::cout << isOk << std::endl;
    bool gs_int = gs.game_server_main(&games, ServSock);
    std::cout << gs_int << std::endl;
    /*bool stop = false;
    std::thread t1(
                [&games, &gc, &stop]()
    {
        while(!stop)
        {
            bool isOk = gc.game_create(&games);
            std::cout << isOk << std::endl;
        }
    });
    std::thread t2(
                [&games, &stop]()
    {
        while(!stop)
        {
            int gs = game_server_main();
            std::cout << gs << std::endl;
        }
    });
    t1.join();
    t2.join();*/

    return 0;
}
