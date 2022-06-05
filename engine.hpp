#pragma once

#include <iostream>
#include <vector>
#include <functional>

namespace engine{

    auto displayPanel(const std::string& fileName,const std::vector<std::function<void()>>& actions) -> void;
    auto getRandomNumber(int a, int b) -> int;
}