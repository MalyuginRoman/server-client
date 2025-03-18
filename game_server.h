#pragma once
#include "dop_function.h"
#include "start_game.h"
#include "order.h"
#include "minor.h"
#include <vector>

class game_server
{
public:
    bool game_server_main(std::vector<game> *games, int gameID, SOCKET ServSock);
    SOCKET bind_server();
};
