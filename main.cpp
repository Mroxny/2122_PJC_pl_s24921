#include <iostream>
#include "creatures.hpp"
#include "engine.hpp"

auto loadMainMenu() -> void;


int main() {
    creatures::special_power s = creatures::special_power("czesc", 10);
    auto a = creatures::creature("Zbyszek", creatures::creature_type::stone, 1, 15, 100, s, 100);
    auto b = creatures::creature("Jedrek", creatures::creature_type::stone, 1, 40, 100, s, 100);

    std::cout<<b.health<<"\n";
    std::string res = a.attack(b)?"Attack succes":"Failed";
    std::cout<< res<<"\n" ;
    std::cout<<b.health<<"\n";



    //loadMainMenu();

    return 0;
}


auto loadMainMenu() -> void{




    int action = 0;
    std::cout<<"Witaj w grze"<<"\n 1.Zacznij Gre \n 2.Wyjdz z gry \n";
    std::cin>>action;

    std::cout<<"Wybrales: "<< action<<"\n";

}
