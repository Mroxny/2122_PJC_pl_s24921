#include "game.hpp"
#include "engine.hpp"
#include <fstream>
#include <utility>

namespace game{

    current_game currentGame;
    player pl;



    auto mainMenu()->void{
        auto menu = std::vector<std::pair<std::string, std::function<void()>>>();
        if(isSaveGameFileValid()){
            menu = {{"LoadGame", []()->void{loadGame();}},
                    {"StartNewGame", []()->void{startNewGame();}},
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

        setDifficulty();
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

    auto setDifficulty() -> void{
        engine::clearScreen();
        engine::displayPanel("difficulty.txt",{
                {"Normal",[]() -> void{currentGame.difficulty = difficulty_modes::normal;}},
                {"Hard",[]() -> void{currentGame.difficulty = difficulty_modes::hard;}}
        });
        selectCreatures();
    }

    auto selectCreatures() -> void{
        auto vec = getCreatures();

        auto team = std::vector<creatures::creature>();

        for(int i = 0; i<3; i++){
            engine::clearScreen();
            std::cout<<"Select Creatures in your team:\n\n";
            auto creatures = std::string();
            for (int j = 0; j<vec.size(); j++){
                engine::displayBorder('=',8);

                creatures =  std::to_string(j+1)+". \n"+ writeCreatureStats(vec[j]);
                std::cout<<creatures;

            }
            engine::displayBorder('=',8);
            int selected;

            do{
                std::cout<<"Write number from 1 to "<<vec.size()<<": ";
                std::cin>>selected;
                if(selected < 1 || selected > vec.size() || !std::cin.good()){
                    std::cout<<"Wrong option\n";
                    selected = 0;
                }
            }while(selected < 1 || selected > vec.size() || !std::cin.good());

            team.push_back(vec[selected-1]);
            vec.erase(vec.begin()+(selected-1));
        }

        acceptCreatures(team);
    }

    auto acceptCreatures(std::vector<creatures::creature> team) -> void{
        auto s = std::string("This is your team:\n");

        for(auto & i : team){
            s+=engine::writeBorder('~', 10);
            s+= writeCreatureStats(i);
        }
        s+=engine::writeBorder('~', 10);


        engine::clearScreen();
        engine::displaySimplePanel(s,{
                {"Accept", [&team]() -> void{pl.setTeam(team); begin();}},
                {"SelectAgain", selectCreatures},

        });

    }

    auto begin() -> void{
        engine::clearScreen();
        showCurrentRound();
        engine::sleep(3000);
        startFight(generateNextEnemy());
    }

    auto showCurrentRound() -> void{
        std::cout<<"Round: "<<currentGame.round<<"\n";
    }

    auto generateNextEnemy() -> enemy{
        return {"Zbyszek",getCreatures()[3]};
    }

    auto startFight(enemy en) -> void{
        auto s = "You fight with " + en.eName + " and his creature: \n";
        s+= writeCreatureStats(en.eCreature);

        engine::clearScreen();
        engine::displaySimplePanel(s,{
                {"Attack", [&en]() -> void{pl.playerTeam[0].attack(en.eCreature);}},
                {"Leave", mainMenu},

        });
        engine::clearScreen();
        std::cout<<"Enemy health: "<<en.eCreature.health;
        engine::sleep(3000);
        engine::clearScreen();


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

    auto writeCreatureStats(const creatures::creature& c) -> std::string{
        auto s= "Name: "+ c.name+"\n"+
                "Type: "+ creatures::creature_type_values[c.type]+"\n"+
                "Strength: "+ std::to_string(c.strength)+"\n"+
                "Health: "+ std::to_string(c.health)+"\n"+
                "Special Power: "+c.sp.name+" ("+
                creatures::special_power_type_values[c.sp.type]+") - "+c.sp.desc+"\n";
        return s;
    }


    auto player::addCreature(const creatures::creature& c) -> void {
        playerTeam.push_back(c);
    }

    auto player::setTeam(std::vector<creatures::creature> team) -> void {
        playerTeam = std::move(team);
    }

    enemy::enemy(std::string name, creatures::creature creature): eName(std::move(name)), eCreature(std::move(creature)) {

    }
}