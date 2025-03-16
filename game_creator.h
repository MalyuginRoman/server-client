#ifndef GAME_CREATOR_H
#define GAME_CREATOR_H
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <QString>
#include <QDir>
#include <QDebug>

#pragma comment(lib, "Ws2_32.lib")

#include "dop_function.h"
#include "minor.h"
#include <vector>

class game_creator
{
public:
    void default_games(std::vector<game> *games);
    int game_create(std::vector<game> *games);
};

#endif // GAME_CREATOR_H
