#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include "creatures.hpp"


namespace game{


    enum class difficulty_modes {
        normal, hard
    };

    struct player{

        std::vector<creatures::creature> playerTeam;
        int currentCreature;

        auto addCreature(const creatures::creature& c) -> void;
        auto setTeam(std::vector<creatures::creature> team) -> void;

    };

    struct enemy{
        enemy(std::string name, creatures::creature creature);

        std::string eName;
        creatures::creature eCreature ;
    };

    struct current_game{

        int currentTour = 1;
        int round = 1;
        int enemyId = 0;

        difficulty_modes difficulty;

    };

    auto mainMenu()->void;
    auto saveGame()->void;
    auto isSaveGameFileValid()->bool;
    auto loadGame()->void;
    auto startNewGame()->void;
    auto exitGame()->void;
    auto info(std::function<void()> prevPanel)->void;
    auto setDifficulty() -> void;
    auto selectCreatures() -> void;
    auto acceptCreatures(std::vector<creatures::creature> team) -> void;
    auto getCreatures() -> std::vector<creatures::creature>;
    auto writeCreatureStats(const creatures::creature& c) -> std::string;
    auto begin() -> void;
    auto showCurrentRound() -> void;
    auto generateNextEnemy() -> enemy;
    auto startFight(enemy en) -> void;


}

