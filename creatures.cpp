#include <iostream>
#include <utility>
#include "creatures.hpp"
#include "engine.hpp"

namespace creatures{


    std::map<creature_type,std::string> creature_type_values = {
            {creature_type::water, "Water"},
            {creature_type::stone, "Stone"},
            {creature_type::air, "Air"},
            {creature_type::fire, "Fire"},
            {creature_type::ice, "Ice"},
            {creature_type::steal, "Steal"}
    };
    std::map<special_power_type,std::string>  special_power_type_values = {
            {special_power_type::defensive, "Defensive"},
            {special_power_type::offensive, "Offensive"}
    };

    special_power::special_power(std::string name,std::string desc, int capacity, special_power_type type, std::function<void()> sp_body)
        : name(std::move(name)),desc(std::move(desc)), capacity(capacity), type(type), sp_body(std::move(sp_body)) {}

    creature::creature(std::string  name, creature_type type, int strength, int skill, int health, special_power  sp, int exp, int expLimit)
        : name(std::move(name)), type(type), strength(strength), skill(skill), health(health), sp(std::move(sp)), exp(exp), expLimit(expLimit) {}

    auto creature::attack(creature &target) const -> bool {
        if( (engine::getRandomNumber(0,100)) > target.skill ){
            target.health-=strength;
            return true;
        }
        else return false;

    }

}