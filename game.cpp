#include "game.hpp"
#include "engine.hpp"
namespace game{
    auto mainMenu()->void{
        auto menu = std::vector<std::pair<std::string, std::function<void()>>>();
        if(isSaveGameFileValid()){
            menu = {{"LoadGame", []()->void{loadGame();}},
                    {"Info", []()->void{info(mainMenu);}},
                    {"Exit", []()->void{exitGame();}}};
        }
        else{
            menu = {{"StartGame", []()->void{startNewGame();}},
                    {"Info", []()->void{info(mainMenu);}},
                    {"Exit", []()->void{exitGame();}}};
        }

        engine::clearScreen();
        engine::displayPanel("main_menu.txt", menu);
    }

    auto isSaveGameFileValid()->bool{
        return false;
    }
    auto loadGame()->void{
        engine::clearScreen();
        std::cout<<"load Game\n";
        engine::sleep(2000);
    }
    auto startNewGame()->void{
        engine::clearScreen();

        std::cout<<"Starting new game...\n";
        engine::sleep(2000);

    }

    auto exitGame()->void{
        engine::clearScreen();

        std::cout<<"Exit Game\n";
        exit(0);
    }

    auto info(std::function<void()> prevPanel)->void{
        auto action = std::vector<std::pair<std::string, std::function<void()>>>();
        action = {{"Back",[&prevPanel]()->void{ prevPanel();}}};

        engine::clearScreen();
        engine::displayPanel("info.txt", action);
    }


}