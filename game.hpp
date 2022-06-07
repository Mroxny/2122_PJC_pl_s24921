#pragma once
#include <iostream>
#include <vector>
#include <functional>


namespace game{

    auto mainMenu()->void;
    auto saveGame()->void;
    auto isSaveGameFileValid()->bool;
    auto loadGame()->void;
    auto startNewGame()->void;
    auto exitGame()->void;
    auto info(std::function<void()> prevPanel)->void;

}

