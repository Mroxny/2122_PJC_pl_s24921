#include <iostream>
#include <random>
#include <functional>

namespace engine{

    auto displayPanel(const std::string& fileName,const std::vector<std::function<void()>>& actions) -> void{
        for(const auto& e : actions) e();
    }



    auto getRandomNumber(int a, int b) -> int{

        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<> distribution(a, b);

        return distribution(gen);
    }

}

