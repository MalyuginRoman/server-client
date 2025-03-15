#ifndef GAME_SERVER_H
#define GAME_SERVER_H
//#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <QString>
#include <QDir>
#include <QDebug>

#pragma comment(lib, "Ws2_32.lib")

#include "dop_function.h"
#include "start_game.h"
#include "order.h"
#include "minor.h"
#include <vector>

class game_server
{
public:
    bool game_server_main(std::vector<game> *games, SOCKET ServSock);
    SOCKET bind_server();
};

#endif // GAME_SERVER_H
