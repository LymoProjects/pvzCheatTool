#include "gameHandler.hpp"

auto main() -> int {
    pvz::gameHandler game;

    game.setMoney(20000);

    sleep::sec(1);
    
    return 0;
}