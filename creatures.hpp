#pragma once

#include <iostream>
#include <utility>

namespace creatures{
    enum class creature_type {
        water, stone, air, fire, ice, steal
    };

    struct special_power{
        std::string desc;
        int capacity;
    };

    struct creature{
        creature(std::string name, creature_type type, int strength, int skill, int health, special_power sp, int exp)
                : name(std::move(name)), type(type), strength(strength), skill(skill), health(health), sp(std::move(sp)), exp(exp) {}


        std::string name;
        creature_type type;

        int strength;
        int skill;
        int health;
        special_power sp;
        int exp;

        auto attack(creature& target) const -> bool {
            if( (std::rand() % 101) > target.skill ){
                target.health-=strength;
                return true;
            }
            else return false;
        }
    };
}