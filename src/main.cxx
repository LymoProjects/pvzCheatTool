#include <iostream>
#include <string>

#include "gameHandler.hpp"

auto main(int argc, char const ** argv) -> int {
    if (argc < 2) {
        std::cout << "you need to input operation.\n";

        return 1;
    }

    pvz::gameHandler game;

    std::string operation(argv[1]);

    if (operation == "setSun") {
        if (argc < 3) {
            std::cout << "you need to input sun count!\n";

            return 1;
        }

        game.setSun(std::stoi(std::string(argv[2])));
    } else if (operation == "lockSun") {
        if (argc < 3) {
            std::cout << "you need to input sun count!\n";

            return 1;
        }
        
        game.lockSun(std::stoi(std::string(argv[2])));
    } else if (operation == "cheatMode") {
        if (argc < 3) {
            std::cout << "you need to input cheat mode avtive boolean!\n";

            return 1;
        }
        
        game.cheatMode(std::stoi(std::string(argv[2])));
    } else if (operation == "toLastAttack") {
        game.toLastAttack();
    } else if (operation == "passLevel") {
        game.passLevel();
    } else if (operation == "setMoney") {
        if (argc < 3) {
            std::cout << "you need to input money!\n";

            return 1;
        }
        
        game.setMoney(std::stoi(std::string(argv[2])));
    } else {
        std::cout << "operation is not working!\n";

        return 1;
    }

    sleep::ms(500);
    
    return 0;
}