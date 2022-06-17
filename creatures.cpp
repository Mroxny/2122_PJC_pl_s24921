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

    special_power::special_power(std::string name,std::string desc, std::string effect,int capacity,special_power_type type, std::function<void()> sp_body)
        : name(std::move(name)),desc(std::move(desc)), effect(std::move(effect)),capacity(capacity), type(type), sp_body(std::move(sp_body)) {
        maxCapacity = capacity;
    }

    creature::creature(std::string  name, creature_type type, int strength, int skill, int health, special_power  sp, int expFroWin, int expLimit)
        : name(std::move(name)), type(type), strength(strength), skill(skill), health(health), sp(std::move(sp)), expFroWin(expFroWin), expLimit(expLimit) {
        maxStrength = strength;
        maxHealth = health;
    }

    auto creature::attack(creature &target) -> int {
        if( (engine::getRandomNumber(0,100)) > target.skill ){
            return takeDamage(target, strength);;
        }
        else return -1;

    }

    auto creature::takeDamage(creature& target, int damage) -> int{
        switch (type) {
            case creature_type::water:
                if(target.type == creature_type::stone ||
                   target.type == creature_type::fire){
                    damage *= 2;
                }
                else if(target.type == creature_type::water){
                    damage /= 2;
                }
                break;
            case creature_type::stone:
                if(target.type == creature_type::fire ||
                   target.type == creature_type::ice ||
                        target.type == creature_type::steal){
                    damage *= 2;
                }
                else if(target.type == creature_type::air){
                    damage /= 2;
                }
                break;
            case creature_type::air:
                if(target.type == creature_type::ice ){
                    damage *= 2;
                }
                else if(target.type == creature_type::stone ||
                        target.type == creature_type::steal){
                    damage /= 2;
                }
                break;
            case creature_type::fire:
                if(target.type == creature_type::ice ||
                   target.type == creature_type::steal){
                    damage *= 2;
                }
                else if(target.type == creature_type::stone ||
                        target.type == creature_type::steal){
                    damage /= 2;
                }
                break;
            case creature_type::ice:
                if(target.type == creature_type::stone){
                    damage *= 2;
                }
                else if(target.type == creature_type::water ||
                        target.type == creature_type::fire ||
                        target.type == creature_type::ice){
                    damage /= 2;
                }
                break;
            case creature_type::steal:
                if(target.type == creature_type::water ||
                   target.type == creature_type::air){
                    damage *= 2;
                }
                else if(target.type == creature_type::fire ||
                        target.type == creature_type::steal){
                    damage /= 2;
                }
                break;
        }
        target.health-=damage;
        if(target.health<0) target.health = 0;
        return damage;
    }

    auto creature::heal(int hp) -> void{
        health+=hp;
    }
    auto creature::heal() -> void{
        heal(maxHealth-health);
    }


    auto creature::addEXP(int earnedEXP) -> void {
        currentExp+=earnedEXP;
    }

    auto creature::upgrade() -> bool {
        int pointsLeft = currentExp - expLimit;
        if(pointsLeft >= 0){
            currentLevel++;
            expLimit += 25;
            currentExp = pointsLeft;
            return true;
        }
        else return false;
    }

    auto creature::levelUp(int levels) -> void {
        currentLevel += levels;

        maxHealth += (levels*10);
        health = maxHealth;
        maxStrength += (levels*5);
        strength = maxStrength;


        expLimit += (levels*25);
    }


}