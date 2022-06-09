#include "game.hpp"
#include "engine.hpp"
#include <fstream>

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
        auto path = std::string("..\\TextFiles\\save_file.txt");
        if(std::ifstream((path)).good()){
            return true;
        }
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

        selectCreatures();

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

    auto selectCreatures() -> void{
        auto vec = getCreatures();

        auto team = std::vector<creatures::creature>();

        for(int i = 0; i<3; i++){
            engine::clearScreen();
            auto creatures = std::string();
            for (int j = 0; j<vec.size(); j++){
                engine::displayBorder('=',8);

                creatures =  std::to_string(j+1)+". \n"+
                            "Name: "+ vec[j].name+"\n"+
                            "Type: "+ creatures::creature_type_values[vec[j].type]+"\n"+
                            "Strength: "+ std::to_string(vec[j].strength)+"\n"+
                            "Health: "+ std::to_string(vec[j].health)+"\n"+
                            "Special Power: "+vec[j].sp.name+"\n";
                std::cout<<creatures;

            }
            engine::displayBorder('=',8);
            int selected;
            std::cout<<"Select your creature no."<<(i+1)<<": ";
            std::cin>>selected;
            team.push_back(vec[selected-1]);
            vec.erase(vec.begin()+(selected-1));
        }

        for(const auto& e : team){
            std::cout<<e.name<<"\n";
        }

        engine::sleep(10000);
    }

    auto getCreatures() -> std::vector<creatures::creature>{
        using namespace creatures;
        auto c_vec = std::vector<creature>();

        special_power s1 = special_power("Wind Tornamdo","Does nothing", 200,special_power_type::defensive, []()-> void {std::cout<<"nothing\n";});
        special_power s2 = special_power("Fireball","Does Fireball", 2,special_power_type::offensive, []()-> void {std::cout<<"fireball!\n";});


        c_vec.emplace_back("Bagno", creatures::creature_type::air, 15, 25, 100, s1, 25, 500);
        c_vec.emplace_back("Vermax", creatures::creature_type::steal, 35, 10, 120, s2, 20, 550);
        c_vec.emplace_back("Michal", creatures::creature_type::water, 20, 15, 110, s1, 22, 480);
        c_vec.emplace_back("Zbyszek", creatures::creature_type::fire, 32, 13, 112, s2, 20, 500);

        return c_vec;

    }


}