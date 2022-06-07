#include <random>
#include <cstdlib>
#include "engine.hpp"

namespace engine{

    auto displayPanel(const std::string& fileName,const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void{
        //system("CLS");
        std::cout<< "display " << fileName<<"\n";
        displayInterface(actions);
    }

    auto displayInterface(const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void {
        std::cout<<"---------------------------\n";
        for (int i = 0; i<actions.size(); i++) {
            std::cout << i << ". "<<actions[i].first<<"\n";
        }
        auto option=std::string();
        do{
            std::cout<< "Write option: ";
            std::cin >> option;
            auto it = std::find_if( actions.begin(), actions.end(),
                                    [&option](const std::pair<std::string , std::function<void()>>& element){ return element.first == option;} );
            if(it != actions.end()){
                system("CLS");
                it->second();
            }
            else if((int&) option < actions.size()) actions[(int&)option].second();
            else {
                std::cout<<"There is no option called "<<option<<"\n";
            }
        }while ();

    }

    auto getRandomNumber(int min, int max) -> int{

        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<> distribution(min, max);

        return distribution(gen);
    }

    auto fun() -> void {
        std::cout<<"TestFun\n";
    }

}

