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

    struct creature;
    struct special_power;


    struct special_power{
        special_power(std::string name,std::string desc, std::string effect ,int capacity, int duration, special_power_type type, std::function<void(creature& owner,creature& target)> sp_body);

        std::string name;
        std::string desc;
        std::string effect;

        int capacity;
        int maxCapacity;

        int duration;
        int endTurn;

        special_power_type type;
        std::function<void(creature& owner,creature& target)> sp_body;
    };

    struct creature{
        creature(std::string  name, creature_type type, int strength, int skill, int health, special_power  sp, int expFroWin, int expLimit);


        std::string name;
        creature_type type;

        int strength;
        int maxStrength;

        int skill;

        float defMultiplier = 1;
        float offMultiplier = 1;


        int health;
        int maxHealth;

        special_power sp;

        int expForWin;
        int expLimit;
        int currentExp = 0;
        int currentLevel = 1;


        auto attack(creature& target) -> int;
        auto takeDamage(creature& target, int damage) -> int;

        auto heal(int hp) -> void;
        auto heal() -> void;


        auto addEXP(int earnedEXP) -> void;
        auto upgrade() -> bool;
        auto levelUp(int levels) -> void;

    };
}