#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <map>

namespace engine{

    auto displayFile(const std::string& path) -> void;
    auto displayBorder(char c, int size) ->void;
    auto displayInterface(const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void;
    auto displayPanel(const std::string& fileName, const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void;
    auto displaySimplePanel(const std::string& header, const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void;
    auto clearScreen() -> void;
    auto sleep(int milliseconds) -> void;

    auto getRandomNumber(int min, int max) -> int;
}