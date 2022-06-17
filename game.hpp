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
        int currentCreature = -1;

        auto addCreature(const creatures::creature& c) -> void;
        auto setTeam(std::vector<creatures::creature> team) -> void;
        auto getCurrentCreature() -> creatures::creature&;

    };

    struct enemy{
        enemy(std::string name, creatures::creature creature);

        std::string eName;
        creatures::creature eCreature ;
    };

    struct current_game{

        int currentTour = 1;
        int round = 1;
        int enemyCount = 1;

        difficulty_modes difficulty = {};
    };

    auto mainMenu()->void;
    auto saveGame()->void;
    auto isSaveGameFileValid()->bool;
    auto loadGame()->void;
    auto startNewGame()->void;
    auto exitGame()->void;
    auto info(std::function<void()> prevPanel)->void;
    auto setDifficulty() -> void;
    auto selectCreaturesInTeam() -> void;
    auto acceptCreatures(std::vector<creatures::creature> team) -> void;
    auto selectCurrentCreature() -> void;
    auto getCreatures() -> std::vector<creatures::creature>;
    auto writeCreatureStats(const creatures::creature& c) -> std::string;
    auto writeCreatureCombatStats(const creatures::creature& c) -> std::string;
    auto nextGame() -> void;
    auto healPlayerTeam() -> void;
    auto generateNextEnemy(int round) -> enemy;
    auto startFight(enemy& en) -> void;
    auto fight(enemy& en) -> void;
    auto attackEnemy(enemy& en) -> void;
    auto useAbility(enemy& en) -> void;
    auto upgradeCreature(enemy& en) -> void;
    auto enemyMove(enemy& en) -> void;
    auto attackPlayer(enemy& en) -> void;
    auto useAbilityOnPlayer(enemy& en) -> void;
    auto upgradeEnemyCreature(enemy& en) -> void;

}

