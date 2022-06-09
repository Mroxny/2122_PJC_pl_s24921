#pragma once

#include <iostream>
#include <utility>
#include <functional>
#include <map>


namespace creatures{
    enum class creature_type {
        water, stone, air, fire, ice, steal
    };

    enum class special_power_type {
            defensive, offensive
    };

    extern std::map<creature_type,std::string> creature_type_values;

    extern std::map<special_power_type,std::string> special_power_type_values;


    struct special_power{
        special_power(std::string name,std::string desc, int capacity, special_power_type type, std::function<void()> sp_body);

        std::string name;
        std::string desc;
        int capacity;
        special_power_type type;
        float multiplier = 1;
        std::function<void()> sp_body;
    };

    struct creature{
        creature(std::string  name, creature_type type, int strength, int skill, int health, special_power  sp, int exp, int expLimit);


        std::string name;
        creature_type type;

        int strength;
        int skill;
        int health;
        special_power sp;
        int exp;
        int expLimit;
        int currentExp = 0;
        int currentLevel = 0;


        auto attack(creature& target) const -> bool;
    };
}