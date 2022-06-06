#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <map>

namespace engine{

    auto displayPanel(const std::string& fileName,const std::map<std::string, std::function<void()>>& actions) -> void;
    auto displayInterface(const std::map<std::string, std::function<void()>>& actions) -> void;
    auto getRandomNumber(int a, int b) -> int;
    auto fun() -> void;
}