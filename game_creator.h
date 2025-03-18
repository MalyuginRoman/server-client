#pragma once
#include "dop_function.h"
#include "minor.h"
#include <vector>

class game_creator
{
public:
    void default_games(std::vector<game> *games);
    int game_create(std::vector<game> *games);
};
