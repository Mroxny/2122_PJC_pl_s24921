#include "engine.hpp"
#include <random>
#include <fstream>
#include <thread>

namespace engine{

    auto HELP_FILE_PATH = std::string("help.txt");
    auto COMMAND_MSG_1 = std::string("Write option: ");
    auto COMMAND_MSG_2 = std::string("Unknown command\n");
    auto afterK = false;


    /**
     *
     * @return returns true if the given string is a positive number
     */
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
            file.close();
        }
        else std::cout<<"File is missing!\n";
    }

    /**
     * Draws a border based on two parameters
     * @param c - char that is used to draw a border
     * @param size - size of a border
     */
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
            std::cout<< COMMAND_MSG_1;
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
            else if(option == "UUDDLRLRBASTART" ||
                    option == "KonamiCode"){
                if(!afterK){
                    std::cout<<"All your base are belong to us!\n";

                    sleep(1000);

                    COMMAND_MSG_1 = "WOLOLO? ";
                    COMMAND_MSG_2 = "Przed wyruszeniem w droge nalezy zebrac druzyne  \n";

                }
                else{
                    std::cout<<"ZARAZA!\n";

                    sleep(1000);

                    COMMAND_MSG_1 = "Write option: ";
                    COMMAND_MSG_2 = "Unknown command\n";
                }
                afterK = !afterK;
                found = false;

            }
            else {
                found = false;
                std::cout<<COMMAND_MSG_2;
            }
        }while (!found);

    }

    /**
     * Displays a file, then prompts the user for the action to be performed
     * @param fileName - name of file to read from.
     * @param actions - functions that program will execute after choice
     */
    auto displayPanel(const std::string& fileName,const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void{
        displayFile(fileName);
        displayInterface(actions);
    }

    /**
     * Displays a header, then prompts the user for the action to be performed
     * @param header - string to display as header
     * @param actions - functions that program will execute after choice
     */
    auto displaySimplePanel(const std::string& header, const std::vector<std::pair<std::string, std::function<void()>>>& actions) -> void{
        std::cout<<header<<"\n";
        displayInterface(actions);
    }

    auto clearScreen() -> void{
        system("CLS");
    }

    /**
     * freezes the program for the given number of milliseconds
     */
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

