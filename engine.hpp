#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <map>

namespace engine{

    auto displayPanel(const std::string& fileName, const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void;
    auto displayInterface(const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void;
    auto getRandomNumber(int min, int max) -> int;
    auto fun() -> void;
}