#include <iostream>
#include <random>

namespace engine{
    auto displayPanel() -> void{
        std::cout<<"ColdEngine 0.1"<<"\n";
    }

    auto getRandomNumber(int a, int b) -> int{

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(a, b); // define the range

        return distr(gen);
    }

}

