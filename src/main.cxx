#include <iostream>

#include "gameHandler.hpp"

auto main() -> int {
    pvz::gameHandler game;

    game.cheatMode(true);

    sleep::sec(1);
    
    return 0;
}