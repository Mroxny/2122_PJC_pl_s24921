#include "engine.hpp"
#include <random>
#include <fstream>
#include <thread>

namespace engine{

    auto HELP_FILE_PATH = std::string("help.txt");

    auto isStringNumber(const std::string& str) -> bool{
        for (char i: str)
            if (i < '0' || i > '9') {
                return false;
            };
        return true;
    }

    auto displayFile(const std::string& path) -> void{
        auto prefix = std::string("..\\TextFiles\\");

        if(std::ifstream((prefix+path)).good()){
            auto file = std::fstream((prefix+path));
            auto line = std::string();

            while (std::getline(file, line)) std::cout<<line<<"\n";
        }
        else std::cout<<"File is missing!\n";
    }

    auto displayBorder(char c, int size) ->void {
        auto res = std::string();
        for(int i = 0; i<size; i++){
            res+=c;
        }
        std::cout<<res<<"\n";
    }
    auto writeBorder(char c, int size) ->std::string{
        auto res = std::string();
        for(int i = 0; i<size; i++){
            res+=c;
        }
        res+="\n";
        return res;
    }


    auto displayInterface(const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void {
        displayBorder('-', 20);
        for (int i = 0; i<actions.size(); i++) {
            std::cout << i+1 << ". "<<actions[i].first<<"\n";
        }
        displayBorder('-', 20);

        bool found;
        do{
            found = true;
            auto option=std::string();
            std::cout<< "Write option: ";
            std::cin >> option;

            auto it = std::find_if( actions.begin(), actions.end(),
                                    [&option](const std::pair<std::string , std::function<void()>>& element){ return element.first == option;} );

            if(it != actions.end()){
                it->second();
            }
            else if(isStringNumber(option) && (std::stoi(option)-1) < actions.size()) {
                actions[std::stoi(option)-1].second();
            }
            else if(option == "-h" ||
                    option == "-help" ||
                    option == "--h" ||
                    option == "--help"){
                found = false;
                displayFile(HELP_FILE_PATH);
            }
            else {
                found = false;
                std::cout<<"Unknown command\n";
            }
        }while (!found);

    }

    auto displayPanel(const std::string& fileName,const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void{
        displayFile(fileName);
        displayInterface(actions);
    }

    auto displaySimplePanel(const std::string& header, const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void{
        std::cout<<header<<"\n";
        displayInterface(actions);
    }

    auto clearScreen() -> void{
        system("CLS");
    }

    auto sleep(int milliseconds) -> void{
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    auto getRandomNumber(int min, int max) -> int{

        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<> distribution(min, max);

        return distribution(gen);
    }


}

