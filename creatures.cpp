#include <iostream>
#include "creatures.hpp"
#include "engine.hpp"

namespace creatures{


    special_power::special_power(std::string desc, int capacity)
        : desc(std::move(desc)), capacity(capacity) {}


    creature::creature(std::string  name, creature_type type, int strength, int skill, int health, special_power  sp, int exp)
        : name(std::move(name)), type(type), strength(strength), skill(skill), health(health), sp(std::move(sp)), exp(exp) {

    }

    auto creature::attack(creature &target) const -> bool {
        if( (engine::getRandomNumber(0,100)) > target.skill ){
            target.health-=strength;
            return true;
        }
        else return false;

    }
}