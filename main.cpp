#include <iostream>
#include <functional>
#include <map>
#include "creatures.hpp"
#include "engine.hpp"






int main() {
    creatures::special_power s = creatures::special_power("czesc", 10);
    auto a = creatures::creature("Zbyszek", creatures::creature_type::stone, 10, 15, 100, s, 100);
    auto b = creatures::creature("Jedrek", creatures::creature_type::stone, 25, 40, 100, s, 100);


    std::cout<<"Creature 1, Health:"<<a.health<<" Skill:"<<a.skill<<"\n";
    std::cout<<"Creature 2, Health:"<<b.health<<" Skill:"<<b.skill<<"\n";

    std::string res1 = a.attack(b)?"Attack 1 Succes":" Attack 1 Failed";
    std::string res2 = b.attack(a)?"Attack 2 Succes":" Attack 2 Failed";

    std::cout<<"Creature 1 attacking Creature 2: "<< res1 <<"\n" ;
    std::cout<<"Creature 2 attacking Creature 1: "<< res2 <<"\n" ;


    std::cout<<"Creature 1, Health:"<<a.health<<" Skill:"<<a.skill<<"\n";
    std::cout<<"Creature 2, Health:"<<b.health<<" Skill:"<<b.skill<<"\n";


    auto vec = std::vector<std::pair<std::string, std::function<void()>>>{
            {"Start", []()->void { std::cout<<"Fun1\n";}},
            {"Options", []()->void { std::cout<<engine::getRandomNumber(1,5)<<"\n";}},
            {"Exit", engine::fun}
    };

    engine::displayPanel("test",vec);

    return 0;
}

