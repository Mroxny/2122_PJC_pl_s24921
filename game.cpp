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

    auto saveGame()->void{

    }

    auto loadGame()->void{
        engine::clearScreen();
        std::cout<<"load Game\n";
        engine::sleep(2000);
    }

    auto startNewGame()->void{
        engine::clearScreen();

        std::cout<<"Starting new game...\n";
        engine::sleep(1000);

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
        selectCreaturesInTeam();
    }

    auto selectCreaturesInTeam() -> void{
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
                {"Accept", [&team]() -> void{pl.setTeam(team); nextGame();}},
                {"SelectAgain", selectCreaturesInTeam},

        });

    }

    auto selectCurrentCreature() -> void{
        auto vec = pl.playerTeam;

        engine::clearScreen();
        std::cout<<"Select your current Creature\n\n";
        auto creatures = std::string();

        for (int j = 0; j<vec.size(); j++){
            engine::displayBorder('=',8);

            creatures =  std::to_string(j+1)+". \n"+ writeCreatureCombatStats(vec[j]);
            std::cout<<creatures;

        }
        engine::displayBorder('=',8);

        int selected;
        bool accept;
        do{
            selected = 0;
            accept = true;
            std::cout<<"Write number from 1 to "<<vec.size()<<": ";
            std::cin>>selected;
            if(selected < 1 || selected > vec.size() || !std::cin.good()){
                std::cout<<"Wrong option\n";
                accept = false;
            }
            else if(pl.playerTeam[(selected-1)].health <= 0){
                std::cout<<pl.playerTeam[(selected-1)].name<<" is too weak right now\n";
                accept = false;
            }

        }while(!accept);
        pl.currentCreature = selected-1;
    }

    auto nextGame() -> void{
        auto rounds = currentGame.difficulty == difficulty_modes::normal? 4 : 5;


        if(currentGame.enemyCount > rounds){
            currentGame.round++;
            currentGame.enemyCount = 1;
            healPlayerTeam();
        }

        engine::clearScreen();

        if(currentGame.round <= rounds){
            enemy en = generateNextEnemy(currentGame.round);

            std::cout<<"Round: "<<currentGame.round<<"\n";
            std::cout<<"Enemy: "<<currentGame.enemyCount<<"\n";

            pl.currentCreature = -1;
            currentGame.currentTour = 1;
            currentGame.enemyCount++;

            engine::sleep(1500);
            startFight(en);
        }

        //game finished
        else{
            std::cout<<"You won!\n";
            engine::sleep(3000);
            info(mainMenu);
        }
    }

    auto healPlayerTeam() -> void {
        for(auto& e: pl.playerTeam){
            e.heal();
            e.strength = e.maxStrength;
        }
    }

    auto generateNextEnemy(int round) -> enemy{
        auto creatures = getCreatures();
        auto multiplier = currentGame.difficulty == difficulty_modes::normal? 1 : 2;
        std::vector<enemy> enemies ={
                {"Jerry",creatures[0]},
                {"Lebron",creatures[0]},
                {"Aron",creatures[1]},
                {"Jax",creatures[1]},
                {"Tony",creatures[2]},
                {"Mac",creatures[2]},
                {"Tom",creatures[3]},
                {"Kate",creatures[3]},
        };
        int eId;
        switch (round) {
            case 1:
                eId = engine::getRandomNumber(0, 1);
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;
                enemies[eId].eCreature.levelUp(-1+multiplier);


                break;
            case 2:
                eId = engine::getRandomNumber(2, 3);
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;
                enemies[eId].eCreature.levelUp(0 + multiplier);
                break;
            case 3:
                eId = engine::getRandomNumber(4, 5);
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;
                enemies[eId].eCreature.levelUp(1 + multiplier);

                break;
            case 4:
                eId = engine::getRandomNumber(6, 7);
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;
                enemies[eId].eCreature.levelUp(2 + multiplier);

                break;
            default:
                eId = engine::getRandomNumber(0, 7);
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;

                break;
        }
        return enemies[eId];
    }

    auto startFight(enemy& en) -> void{
        auto s = "You fight with " + en.eName + " and his creature: \n\n";
        s+= writeCreatureStats(en.eCreature);

        engine::clearScreen();
        engine::displaySimplePanel(s,{
                {"SelectCreature", [&en]() -> void{selectCurrentCreature(); fight(en);}},
                {"Save&Exit", []() -> void{
                    saveGame();

                    currentGame.currentTour = 1;
                    currentGame.round = 1;
                    currentGame.enemyCount = 1;

                    mainMenu();}},

        });
    }

    auto fight(enemy& en) -> void{
        engine::clearScreen();

        if(pl.getCurrentCreature().health <= 0){
            auto exp = en.eCreature.expFroWin;
            en.eCreature.addEXP(exp);

            std::cout<<pl.getCurrentCreature().name<<" passed out!\n"
                     <<"You have to choose different creature.";

            engine::sleep(2000);

            selectCurrentCreature();
            fight(en);
        }

        else if(en.eCreature.health <= 0){

            auto exp = pl.getCurrentCreature().expFroWin;
            pl.getCurrentCreature().addEXP(exp);
            pl.getCurrentCreature().heal(en.eCreature.strength);

            std::cout<< "You defeated "+en.eName+"!\n"
                     << "Earned "+ std::to_string(exp) + " EXP";

            engine::sleep(3000);
            nextGame();
        }

        else{
            engine::displayBorder('+', 8);
            std::cout<<"Tour: "<<currentGame.currentTour<<"\n";
            engine::displayBorder('+', 8);
            auto out = std::string("\n");

            out += "Your creature:\n";
            out += writeCreatureCombatStats(pl.getCurrentCreature())+"\n";

            out += std::to_string(pl.getCurrentCreature().currentExp) + " / " + std::to_string(pl.getCurrentCreature().expLimit) + " EXP\n";
            out += std::to_string(pl.getCurrentCreature().sp.capacity)+" special abilities left.\n\n";

            out += engine::writeBorder('#', 20) + "\n";

            out += "Enemy's creature:\n";
            out += writeCreatureCombatStats(en.eCreature);


            currentGame.currentTour++;
            engine::displaySimplePanel(out,{
                    {"Attack", [&en]()-> void{attackEnemy(en); enemyMove(en); fight(en);}},
                    {"UseAbility", [&en]()-> void{useAbility(en); enemyMove(en); fight(en);}},
                    {"UpgradeCreature", [&en]()-> void{upgradeCreature(en); enemyMove(en); fight(en);}},
                    {"ChangeCreature", [&en]()-> void{selectCurrentCreature(); enemyMove(en); fight(en);}},

            });
        }
    }

    auto attackEnemy(enemy& en) -> void{
        engine::clearScreen();

        auto damage = pl.getCurrentCreature().attack(en.eCreature);
        if(damage != -1){
            auto exp = 25;

            pl.getCurrentCreature().addEXP(exp);

            std::cout<<"Attack successful\n"
                     <<"Damage: "<< damage <<"\n"
                     <<"Earned "<< exp << " EXP";
        }
        else{
            std::cout<<"Attack failed\n";
        }
        engine::sleep(2000);

    }

    auto useAbility(enemy& en) -> void{
        engine::clearScreen();
        creatures::special_power sp = pl.getCurrentCreature().sp;
        if(sp.capacity > 0){
            int exp = 50;

            pl.getCurrentCreature().addEXP(exp);

            std::cout<<sp.effect<<"\n"
                     <<"Earned "<< exp << " EXP";

            engine::sleep(2000);
        }
        else {
            auto out = std::string("Special power is not available\n");

            engine::displaySimplePanel(out,{
                    {"Attack", [&en]()-> void{attackEnemy(en);}},
                    {"UpgradeCreature", [&en]()-> void{upgradeCreature(en);}},
                    {"ChangeCreature", []()-> void{selectCurrentCreature();}},

            });
        }

    }

    auto upgradeCreature(enemy& en) -> void{
        if(pl.getCurrentCreature().upgrade()){
            auto skills = std::vector<std::string>{"Health points", "Strength", "Skill", "EXP"};
            for(int i = 0; i<2; i++){
                engine::clearScreen();

                std::cout<<"Select attributes to upgrade in "<<"\n\n";

                std::cout<<writeCreatureCombatStats(pl.getCurrentCreature())<<"\n";

                for (int j = 0; j<skills.size(); j++){
                    engine::displayBorder('=',16);
                    std::cout<<(j+1)<<". "<<skills[j]<<"\n";
                }
                engine::displayBorder('=',16);

                int selected;
                bool accept;
                do{
                    selected = 0;
                    accept = true;
                    std::cout<<"Write number from 1 to "<<skills.size()<<": ";
                    std::cin>>selected;
                    if(selected < 1 || selected > skills.size() || !std::cin.good()){
                        std::cout<<"Wrong option\n";
                        accept = false;
                    }
                }while(!accept);

                switch (selected) {
                    case 1:
                        pl.getCurrentCreature().maxHealth += 10;
                        break;
                    case 2:
                        pl.getCurrentCreature().maxStrength += 5;
                        pl.getCurrentCreature().strength = pl.getCurrentCreature().maxStrength;
                    case 3:
                        pl.getCurrentCreature().skill+=5;
                    case 4:
                        pl.getCurrentCreature().expFroWin+=10;
                    default:
                        break;
                }
            }
        }
        else {
            auto out = std::string(pl.getCurrentCreature().name+" dont have enough EXP to upgrade\n");

            engine::displaySimplePanel(out,{
                    {"Attack", [&en]()-> void{attackEnemy(en);}},
                    {"UseAbility", [&en]()-> void{useAbility(en);}},
                    {"ChangeCreature", []()-> void{selectCurrentCreature();}},

            });
        }
    }

    auto enemyMove(enemy& en) -> void{
        if(en.eCreature.health > 0) {
            auto range = en.eCreature.sp.capacity > 0 ? 3 : 2;
            auto choice = engine::getRandomNumber(1, range);

            switch (choice) {
                case 1:
                    attackPlayer(en);
                    break;

                case 2:
                    upgradeEnemyCreature(en);
                    break;

                case 3:
                    useAbilityOnPlayer(en);
                    break;

                default:
                    attackPlayer(en);
                    break;
            }
        }
    }

    auto attackPlayer(enemy& en) -> void{
        engine::clearScreen();

        auto damage = en.eCreature.attack(pl.getCurrentCreature());
        if(damage != -1){
            auto exp = 25;

            en.eCreature.addEXP(exp);

            std::cout<<en.eName<<"'s move: Attack\n"
                     <<"Attack successful\n"
                     <<"Damage: "<< damage <<"\n";
        }
        else{
            std::cout<<en.eName<<"'s move: Attack\n"
                     <<"Attack failed\n";
        }
        engine::sleep(2000);
    }

    auto useAbilityOnPlayer(enemy& en) -> void{
        engine::clearScreen();
        creatures::special_power sp = en.eCreature.sp;

        if(sp.capacity > 0){
            int exp = 50;

            en.eCreature.addEXP(exp);

            std::cout<<en.eName<<"'s move: Special Ability\n";
            std::cout<<sp.effect;
            engine::sleep(2000);
        }

        else attackPlayer(en);
    }

    auto upgradeEnemyCreature(enemy& en) -> void{
        if(en.eCreature.upgrade()){

            std::cout<<en.eName<<"'s move: Upgrade Creature\n";
            en.eCreature.maxHealth += 10;
            en.eCreature.maxStrength += 5;
            en.eCreature.strength = en.eCreature.maxStrength;
            en.eCreature.skill += 5;

            engine::sleep(2000);
        }
        else attackPlayer(en);
    }


    auto getCreatures() -> std::vector<creatures::creature>{
        using namespace creatures;
        auto c_vec = std::vector<creature>();

        special_power s1 = special_power("Wind Tornamdo","Does nothing", "Woosh!",0,special_power_type::defensive, []()-> void {std::cout<<"nothing\n";});
        special_power s2 = special_power("Fireball","Does Fireball", "Booom!",2,special_power_type::offensive, []()-> void {std::cout<<"fireball!\n";});


        c_vec.emplace_back("Bagno", creatures::creature_type::air, 15, 25, 100, s1, 100, 250);
        c_vec.emplace_back("Vermax", creatures::creature_type::steal, 35, 10, 120, s2, 110, 275);
        c_vec.emplace_back("Michal", creatures::creature_type::water, 20, 15, 110, s1, 80, 240);
        c_vec.emplace_back("Zbyszek", creatures::creature_type::fire, 32, 13, 112, s2, 100, 250);

        return c_vec;

    }

    auto writeCreatureStats(const creatures::creature& c) -> std::string{
        auto s= "Name: "+ c.name+"\n"+
                "Health: "+ std::to_string(c.health)+"\n"+
                "Strength: "+ std::to_string(c.strength)+"\n"+
                "Skill: "+ std::to_string(c.skill)+"\n"+
                "Level: "+ std::to_string(c.currentLevel)+"\n"+
                "Type: "+ creatures::creature_type_values[c.type]+"\n"+
                "Special Power: "+c.sp.name+" ("+
                creatures::special_power_type_values[c.sp.type]+") - "+c.sp.desc+"\n";
        return s;
    }

    auto writeCreatureCombatStats(const creatures::creature& c) -> std::string{
        auto s= "Name: "+ c.name+"\n"+
                "Health: "+ std::to_string(c.health)+" / "+std::to_string(c.maxHealth)+"\n"+
                "Strength: "+ std::to_string(c.strength)+" / "+std::to_string(c.maxStrength)+"\n"+
                "Skill: "+ std::to_string(c.skill)+"\n"+
                "Level: "+ std::to_string(c.currentLevel)+"\n"+
                "Type: "+ creatures::creature_type_values[c.type]+"\n"+
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

    auto player::getCurrentCreature() -> creatures::creature& {
        return pl.playerTeam[pl.currentCreature];
    }

    enemy::enemy(std::string name, creatures::creature creature): eName(std::move(name)), eCreature(std::move(creature)) {

    }
}