#include <random>
#include <cstdlib>
#include "engine.hpp"

namespace engine{

    auto displayPanel(const std::string& fileName,const std::map<std::string, std::function<void()>>& actions) -> void{
        system("CLS");
        std::cout<< "display " << fileName<<"\n";
        displayInterface(actions);
    }

    auto displayInterface(const std::map<std::string, std::function<void()>>& actions) -> void {
        std::cout<<"---------------------------\n";
        int i =0;
        for (const auto& [key, value] : actions) {
            i++;
            std::cout << i << ". "<<key<<"\n";
        }
        std::string option;


        do{
            std::cout<< "Write option: ";
            std::cin >> option;
            if(actions.contains(option)){

                actions.find(option) -> second();
            }
            else {
                std::cout<<"There is no option called "<<option<<"\n";


            }
        }while (!actions.contains(option));

    }

    auto getRandomNumber(int a, int b) -> int{

        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<> distribution(a, b);

        return distribution(gen);
    }

    auto fun() -> void {
        std::cout<<"TestFun\n";
    }

}

