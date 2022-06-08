#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include "creatures.hpp"


namespace game{

    struct entity{
        virtual auto make_move() -> void = 0;

        virtual ~entity() = default;
    };

    struct player : entity{
        std::vector<creatures::creature> team;

        auto make_move() -> void override;
    };
    struct enemy : entity{
        auto make_move() -> void override;
    };

    struct current_game{

        int currentTour;
        int round;
        int enemyId;
        std::unique_ptr<entity> player;
        std::unique_ptr<entity> currentEnemy;

    };

    auto mainMenu()->void;
    auto saveGame()->void;
    auto isSaveGameFileValid()->bool;
    auto loadGame()->void;
    auto startNewGame()->void;
    auto exitGame()->void;
    auto info(std::function<void()> prevPanel)->void;

}

