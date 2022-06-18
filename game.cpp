#include "game.hpp"
#include "engine.hpp"
#include <fstream>
#include <utility>

namespace game{

    auto saveFilePath = std::string("..\\TextFiles\\save_file.txt");

    std::map<difficulty_modes,std::string>  difficulty_modes_values = {
            {difficulty_modes::normal, "Normal"},
            {difficulty_modes::hard, "Hard"}
    };

    current_game currentGame;
    player pl;


    auto mainMenu()->void{
        auto menu = std::vector<std::pair<std::string, std::function<void()>>>();
        if(isSaveGameFileValid()){
            menu = {{"LoadGame", []()->void{loadGame();}},
                    {"StartNewGame", []()->void{destroySaveFile();}},
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

    /**
     * Starts a new game and deletes the old one if the player decides to start over
     */
    auto destroySaveFile()->void{
        engine::clearScreen();
        //bardzo bym chcial zmienic string na const char* ale niestety nie umiem :<

        auto c = "..\\TextFiles\\save_file.txt";
        engine::displayPanel("new_game.txt",{
                {"Yes",[&c]()->void{ remove(c); startNewGame();}},
                {"No",mainMenu}
        });
    }

    auto isSaveGameFileValid()->bool{

        return std::ifstream(saveFilePath).good();
    }

    /**
     * Saves the state of the game
     */
    auto saveGame()->void{
        auto file = std::fstream(saveFilePath, std::ios::out | std::ios::trunc);
        file<<"current_game:\n";
        file<<currentGame.round<<"\n";
        file<<(currentGame.enemyCount-1)<<"\n";
        file<<difficulty_modes_values[currentGame.difficulty]<<"\n";

        auto creatures = getCreatures();

        file<<"player:\n";
        for(const auto& e: pl.playerTeam){
            auto name  = e.name;

            auto it = std::find_if( creatures.begin(), creatures.end(),
                                    [&name](const creatures::creature& c){ return c.name == name;} );
            auto index = std::distance(creatures.begin(), it);

            file<<index<<"\n";
            file<<e.strength<<"\n";
            file<<e.maxStrength<<"\n";
            file<<e.skill<<"\n";
            file<<e.health<<"\n";
            file<<e.maxHealth<<"\n";
            file<<e.expLimit<<"\n";
            file<<e.currentExp<<"\n";
            file<<e.currentLevel<<"\n";
        }

        file.close();
    }

    /**
     * Reads individual values from a file and puts them into variables
     */
    auto loadGame()->void{

        if(isSaveGameFileValid()){
            auto file = std::fstream(saveFilePath);
            auto line = std::string();
            std::getline(file, line);

            if(line == "current_game:"){
                std::getline(file, line);
                currentGame.round = std::stoi(line);

                std::getline(file, line);
                currentGame.enemyCount = std::stoi(line);

                std::getline(file, line);
                if(line == difficulty_modes_values[difficulty_modes::normal])
                    currentGame.difficulty = difficulty_modes::normal;
                else if(line == difficulty_modes_values[difficulty_modes::hard])
                    currentGame.difficulty = difficulty_modes::hard;

            }

            std::getline(file, line);

            if(line == "player:"){
                auto allCreatures = getCreatures();
                auto team = std::vector<creatures::creature>();
                for(int i=0; i<6; i++){
                    std::getline(file, line);
                    auto c = allCreatures[std::stoi(line)];

                    std::getline(file, line);
                    c.strength = std::stoi(line);

                    std::getline(file, line);
                    c.maxStrength = std::stoi(line);

                    std::getline(file, line);
                    c.skill = std::stoi(line);

                    std::getline(file, line);
                    c.health = std::stoi(line);

                    std::getline(file, line);
                    c.maxHealth = std::stoi(line);

                    std::getline(file, line);
                    c.expLimit = std::stoi(line);

                    std::getline(file, line);
                    c.currentExp = std::stoi(line);

                    std::getline(file, line);
                    c.currentLevel = std::stoi(line);

                    team.push_back(c);
                }
                pl.setTeam(team);
            }

            file.close();
        }
        nextGame();
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

    /**
     * Displays the panel when the player has lost
     */
    auto gameOver() -> void{
        engine::clearScreen();

        engine::displayFile("you_lost.txt");
        engine::sleep(2000);
        engine::displaySimplePanel("What now?\n",{
                {"TryAgain",[]()->void{healPlayerTeam(); loadGame();}},
                {"StartNewGame",destroySaveFile},
                {"Quit",mainMenu}
        });
    }

    /**
     * Displays a panel with information from the author
     * @param prevPanel - link to the previous panel
     */
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

    /**
     * Creates the player's team
     */
    auto selectCreaturesInTeam() -> void{
        auto vec = getCreatures();

        auto team = std::vector<creatures::creature>();

        for(int i = 0; i<6; i++){
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

    /**
     * Provides the ability to select creatures for the player's team again
     * @param team - selected team
     */
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

    /**
     * Prepares the next opponent
     */
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
            for(auto& e: pl.playerTeam){
                e.sp.capacity = e.sp.maxCapacity;
                e.sp.endTurn = -1;
            }

            engine::sleep(1500);
            startFight(en);
        }

        //game finished
        else{
            engine::displayFile("you_won.txt");
            engine::sleep(3000);
            info(mainMenu);
        }
    }

    /*
     * Heals the base attributes of each creature in the player's party
     */
    auto healPlayerTeam() -> void {
        for(auto& e: pl.playerTeam){
            e.heal();
            e.strength = e.maxStrength;
            e.sp.capacity = e.sp.maxCapacity;
            e.sp.endTurn = -1;
        }
    }

    /**
     * Depending on the current round, returns a opponent
     * @param round - variable for calculation
     * @return
     */
    auto generateNextEnemy(int round) -> enemy{
        auto creatures = getCreatures();
        auto multiplier = currentGame.difficulty == difficulty_modes::normal? 1 : 1.5;
        std::vector<enemy> enemies ={
                //Round 1
                {"Jerry",creatures[0]},
                {"Lebron",creatures[3]},
                {"Aron",creatures[6]},
                {"Jax",creatures[12]},
                //Round 2
                {"Tony",creatures[1]},
                {"Mac",creatures[4]},
                {"Tom",creatures[7]},
                {"Kate",creatures[13]},
                //Round 3
                {"Alex",creatures[2]},
                {"Andrew",creatures[5]},
                {"Aron",creatures[11]},
                {"Sam",creatures[14]},
                //Round 4
                {"Tony's mom",creatures[9]},
                {"Karen",creatures[10]},
                {"Michael",creatures[16]},
                {"Jackson",creatures[17]},

                {"Jerry's dad",creatures[0]}
        };
        int eId;
        switch (round) {
            case 1:
                eId = currentGame.enemyCount-1;
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;
                enemies[eId].eCreature.levelUp(0);
                break;

            case 2:
                eId = currentGame.enemyCount+3;
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;
                enemies[eId].eCreature.levelUp(1);
                break;

            case 3:
                eId = currentGame.enemyCount+7;
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;
                enemies[eId].eCreature.levelUp(2);

                break;
            case 4:
                eId = currentGame.enemyCount+11;
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;
                enemies[eId].eCreature.levelUp(3);

                break;
            default:
                eId = engine::getRandomNumber(0, enemies.size()-1);
                enemies[eId].eCreature.maxStrength *= multiplier;
                enemies[eId].eCreature.strength = enemies[eId].eCreature.maxStrength;
                enemies[eId].eCreature.skill *= multiplier;

                break;
        }
        return enemies[eId];
    }

    /**
     * It displays the basic attributes of the opponent and gives the player the ability to save and exit
     * @param en - player's opponent (enemy)
     */
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

    /**
     * basic game loop, determines whether the player or the opponent has won
     * @param en - player's opponent (enemy)
     */
    auto fight(enemy& en) -> void{
        engine::clearScreen();

        if(pl.getCurrentCreature().health <= 0){
            auto exp = en.eCreature.expForWin;
            en.eCreature.addEXP(exp);

            std::cout<<pl.getCurrentCreature().name<<" passed out!\n";
            engine::sleep(2000);

            std::vector<creatures::creature> c = pl.playerTeam;
            std::sort(c.begin(), c.end(),[](const creatures::creature& a, const creatures::creature& b)->bool{return a.health > b.health;});

            if(c[0].health <= 0){
                gameOver();
            }
            else{
                std::cout<<"You have to choose different creature.\n";
                engine::sleep(2000);

                selectCurrentCreature();
                fight(en);
            }
        }

        else if(en.eCreature.health <= 0){

            auto exp = pl.getCurrentCreature().expForWin;
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
            out += std::to_string(pl.getCurrentCreature().sp.capacity)+" special abilities left.\n";
            int endT = pl.getCurrentCreature().sp.endTurn;
            if(endT > currentGame.currentTour){
                out += "*Special power active for "+ std::to_string( endT - currentGame.currentTour) +" turns*\n";
            }
            out +="\n";

            out += engine::writeBorder('#', 20) + "\n";

            out += en.eName+"'s creature:\n";
            out += writeCreatureCombatStats(en.eCreature);


            currentGame.currentTour++;
            engine::displaySimplePanel(out,{
                    {"Attack", [&en]()-> void{attackEnemy(en); checkSpecialPowers(en); enemyMove(en); fight(en);}},
                    {"UseAbility", [&en]()-> void{useAbility(en); enemyMove(en); fight(en);}},
                    {"UpgradeCreature", [&en]()-> void{upgradeCreature(en); checkSpecialPowers(en); enemyMove(en); fight(en);}},
                    {"ChangeCreature", [&en]()-> void{selectCurrentCreature(); checkSpecialPowers(en); enemyMove(en); fight(en);}},

            });
        }
    }

    /**
     * checks if the player's and opponent's special powers are still active
     * @param en - player's opponent (enemy)
     */
    auto checkSpecialPowers(enemy& en) -> void{
        creatures::special_power& sp1 = pl.getCurrentCreature().sp;
        if(sp1.endTurn <= (currentGame.currentTour-1)){
            sp1.endTurn = -1;
            if(sp1.type == creatures::special_power_type::defensive){
                pl.getCurrentCreature().defMultiplier = 1;
            }
            if(sp1.type == creatures::special_power_type::offensive){
                pl.getCurrentCreature().offMultiplier = 1;
            }
        }
        else if(sp1.endTurn >= 0){
            sp1.sp_body(pl.getCurrentCreature() ,en.eCreature);
            if(sp1.type == creatures::special_power_type::offensive){
                engine::clearScreen();
                std::cout<<sp1.effect<<"\n";
                engine::sleep(2000);
            }

        }

        if(en.eCreature.health > 0) {
            creatures::special_power& sp2 = en.eCreature.sp;
            if (sp2.endTurn <= (currentGame.currentTour - 1)) {
                sp2.endTurn = -1;
                if (sp2.type == creatures::special_power_type::defensive) {
                    en.eCreature.defMultiplier = 1;
                }
                if (sp2.type == creatures::special_power_type::offensive) {
                    en.eCreature.offMultiplier = 1;
                }
            } else if (sp2.endTurn >= 0)
                sp2.sp_body(en.eCreature, pl.getCurrentCreature());
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
        creatures::special_power& sp = pl.getCurrentCreature().sp;
        if(sp.capacity > 0 && sp.endTurn <= currentGame.currentTour-1){
            int exp = 50;

            pl.getCurrentCreature().addEXP(exp);

            std::cout<<sp.name<<"\n"
                     <<sp.effect<<"\n"
                     <<"Earned "<< exp << " EXP";

            sp.sp_body(pl.getCurrentCreature() ,en.eCreature);
            sp.endTurn = currentGame.currentTour + sp.duration;

            sp.capacity--;

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

                std::cout<<"Upgrade points: "<<(2-i)<<"\n";
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
                        pl.getCurrentCreature().skill+=1;
                    case 4:
                        pl.getCurrentCreature().expForWin+=10;
                    default:
                        break;
                }
            }
        }
        else {
            auto out = std::string(pl.getCurrentCreature().name+" dont have enough EXP to upgrade\n");

            engine::clearScreen();
            engine::displaySimplePanel(out,{
                    {"Attack", [&en]()-> void{attackEnemy(en);}},
                    {"UseAbility", [&en]()-> void{useAbility(en);}},
                    {"ChangeCreature", []()-> void{selectCurrentCreature();}},

            });
        }
    }

    /**
     * decides what action the opponent should take
     * @param en - player's opponent (enemy)
     */
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
        creatures::special_power& sp = en.eCreature.sp;

        if(sp.capacity > 0){
            int exp = 50;

            en.eCreature.addEXP(exp);

            std::cout<<en.eName<<"'s move: Special Ability\n"
                     <<sp.name<<"\n"
                     <<sp.effect<<"\n";

            sp.sp_body(en.eCreature ,pl.getCurrentCreature());
            sp.endTurn = currentGame.currentTour + sp.duration;

            sp.capacity--;

            engine::sleep(2000);
        }

        else attackPlayer(en);
    }

    auto upgradeEnemyCreature(enemy& en) -> void{
        if(en.eCreature.upgrade()){
            engine::clearScreen();
            std::cout<<en.eName<<"'s move: Upgrade Creature\n";
            en.eCreature.maxHealth += 10;
            en.eCreature.maxStrength += 5;
            en.eCreature.strength = en.eCreature.maxStrength;
            en.eCreature.skill += 5;

            engine::sleep(2000);
        }
        else attackPlayer(en);
    }

    /**
     *
     * @return generates a vector of creatures available throughout the game
     */
    auto getCreatures() -> std::vector<creatures::creature>{
        using namespace creatures;

        //water
        special_power s1 = special_power("Water Blades","Takes 20HP from the opponent for 2 turns", "Woosh  Woosh  Woosh!",2,2,special_power_type::offensive, [](creature& owner,creature& target)-> void {owner.takeDamage(target,20);});
        special_power s2 = special_power("Water Shield","Reduces damage to the owner by 25% for 4 turns", "*Waterfall sound*",2,4,special_power_type::defensive, [](creature& owner,creature& target)-> void {owner.defMultiplier = .75;});
        //stone
        special_power s3 = special_power("Meteor","Takes 50% of current HP from the opponent once", "Booooom!",2,0,special_power_type::offensive, [](creature& owner,creature& target)-> void {owner.takeDamage(target, target.health*0.5);});
        special_power s4 = special_power("Stone Armor","Reduces damage to the owner by 50% for 1 turn", "Click Clack Click",4,1,special_power_type::defensive, [](creature& owner,creature& target)-> void {owner.defMultiplier = .5;});
        //air
        special_power s5 = special_power("Wind Tornado","Takes 40HP from the opponent once", "*Coming storm*",4,0,special_power_type::offensive, [](creature& owner,creature& target)-> void {owner.takeDamage(target,40);});
        special_power s6 = special_power("Air Capsule","Permanently adds 2 skill points to the owner", "Swing!",2,0,special_power_type::defensive, [](creature& owner,creature& target)-> void {owner.skill+= 2;});
        //fire
        special_power s7 = special_power("Fireball","Takes 45HP from the opponent once", "Poof!",3,0,special_power_type::offensive, [](creature& owner,creature& target)-> void {owner.takeDamage(target,45);});
        special_power s8 = special_power("Healing Fire","Gives to the owner 30 HP once", "Pow!",2,0,special_power_type::defensive, [](creature& owner,creature& target)-> void {owner.heal(30);});
        //ice
        special_power s9 = special_power("Ice Sword","The owner deals 30% more damage for 3 turns", "Wham!",2,3,special_power_type::offensive, [](creature& owner,creature& target)-> void {owner.offMultiplier = 1.3;});
        special_power s10 = special_power("Ice Shield","Permanently adds 5 skill points to the owner", "Whiiish!",1,0,special_power_type::defensive, [](creature& owner,creature& target)-> void {owner.skill+= 5;});
        //steal
        special_power s11 = special_power("Sharp Chainmail","The owner deals 45% more damage for 1 turn", "Swish!",1,1,special_power_type::offensive, [](creature& owner,creature& target)-> void {owner.offMultiplier = 1.45;});
        special_power s12 = special_power("Healthy Gold","Gives to the owner 20 HP for 2 turns", "Zap!",2,2,special_power_type::defensive, [](creature& owner,creature& target)-> void {owner.heal(20);});

        auto c_vec = std::vector<creature>{
                //water
                {"Luna", creature_type::water, 18, 20, 120, s2, 110, 230},
                {"Aquer", creature_type::water, 22, 22, 110, s1, 110, 230},
                {"Thunder", creature_type::water, 20, 25, 115, s2, 110, 230},

                //stone
                {"Bango", creature_type::stone, 18, 10, 150, s4, 100, 250},
                {"Gormix", creature_type::stone, 15, 15, 135, s3, 100, 240},
                {"Zipper", creature_type::stone, 20, 10, 145, s4, 100, 250},

                //air
                {"Windy", creature_type::air, 20, 25, 100, s6, 90, 250},
                {"Clouder", creature_type::air, 15, 22, 110, s5, 90, 240},
                {"Enchu", creature_type::air, 20, 25, 120, s6, 90, 250},

                //fire
                {"Magma", creature_type::fire, 21, 18, 122, s7, 85, 230},
                {"Fenix", creature_type::fire, 19, 18, 135, s8, 100, 230},
                {"Smoke", creature_type::fire, 22, 18, 120, s7, 85, 230},

                //ice
                {"Glacier", creature_type::ice, 21, 20, 125, s9, 110, 230},
                {"Slider", creature_type::ice, 19, 20, 130, s10, 100, 230},
                {"Nepto", creature_type::ice, 22, 18, 125, s9, 115, 230},

                //steal
                {"Tytanus", creature_type::steal, 22, 10, 155, s11, 90, 270},
                {"Gold hammer", creature_type::steal, 24, 13, 140, s12, 90, 270},
                {"Iron Golem", creature_type::steal, 25, 12, 140, s11, 95, 270}
        };

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
        int stren = c.strength * c.offMultiplier;
        auto s= "Name: "+ c.name+"\n"+
                "Health: "+ std::to_string(c.health)+" / "+std::to_string(c.maxHealth)+"\n"+
                "Strength: "+ std::to_string(stren)+" / "+std::to_string(c.maxStrength)+"\n"+
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