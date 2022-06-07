#pragma once

#include <iostream>
#include <utility>

namespace creatures{
    enum class creature_type {
        water, stone, air, fire, ice, steal
    };

    enum class special_power_type {
            defensive, offensive
    };

    struct special_power{
        special_power(std::string desc, int capacity, special_power_type type);

        std::string desc;
        int capacity;
        special_power_type type;
    };

    struct creature{
        creature(std::string  name, creature_type type, int strength, int skill, int health, special_power  sp, int exp);


        std::string name;
        creature_type type;

        int strength;
        int skill;
        int health;
        special_power sp;
        int exp;

        auto attack(creature& target) const -> bool;
    };
}