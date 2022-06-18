#include <iostream>
#include <utility>
#include "creatures.hpp"
#include "engine.hpp"

namespace creatures{

    /**
     * Global maps for better management of enums
     */
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

    special_power::special_power(std::string name,std::string desc, std::string effect,int capacity, int duration, special_power_type type, std::function<void(creature& owner,creature& target)>  sp_body)
        : name(std::move(name)),desc(std::move(desc)), effect(std::move(effect)),capacity(capacity), duration(duration), type(type), sp_body(std::move(sp_body)) {
        maxCapacity = capacity;
        endTurn = -1;
    }

    creature::creature(std::string  name, creature_type type, int strength, int skill, int health, special_power  sp, int expForWin, int expLimit)
        : name(std::move(name)), type(type), strength(strength), skill(skill), health(health), sp(std::move(sp)), expForWin(expForWin), expLimit(expLimit) {
        maxStrength = strength;
        maxHealth = health;
    }

    /**
     *
     * @return returns the damage dealt if it manages to attack
     */
    auto creature::attack(creature &target) -> int {
        if( (engine::getRandomNumber(0,100)) > target.skill ){
            return takeDamage(target, strength);;
        }
        else return -1;

    }

    /**
     *
     * @return returns the damage dealt
     */
    auto creature::takeDamage(creature& target, int damage) -> int{
        auto multiplier = 1.5;
        switch (type) {
            case creature_type::water:
                if(target.type == creature_type::stone ||
                   target.type == creature_type::fire){
                    damage *= multiplier;
                }
                else if(target.type == creature_type::water){
                    damage /= multiplier;
                }
                break;
            case creature_type::stone:
                if(target.type == creature_type::fire ||
                   target.type == creature_type::ice ||
                        target.type == creature_type::steal){
                    damage *= multiplier;
                }
                else if(target.type == creature_type::air){
                    damage /= multiplier;
                }
                break;
            case creature_type::air:
                if(target.type == creature_type::ice ){
                    damage *= multiplier;
                }
                else if(target.type == creature_type::stone ||
                        target.type == creature_type::steal){
                    damage /= multiplier;
                }
                break;
            case creature_type::fire:
                if(target.type == creature_type::ice ||
                   target.type == creature_type::steal){
                    damage *= multiplier;
                }
                else if(target.type == creature_type::stone ||
                        target.type == creature_type::steal){
                    damage /= multiplier;
                }
                break;
            case creature_type::ice:
                if(target.type == creature_type::stone){
                    damage *= multiplier;
                }
                else if(target.type == creature_type::water ||
                        target.type == creature_type::fire ||
                        target.type == creature_type::ice){
                    damage /= multiplier;
                }
                break;
            case creature_type::steal:
                if(target.type == creature_type::water ||
                   target.type == creature_type::air){
                    damage *= multiplier;
                }
                else if(target.type == creature_type::fire ||
                        target.type == creature_type::steal){
                    damage /= multiplier;
                }
                break;
        }
        damage *= offMultiplier;
        damage *= target.defMultiplier;
        target.health-=damage;
        if(target.health<0) target.health = 0;
        return damage;
    }

    /**
     *
     * heals the creature while keeping to the limit
     */
    auto creature::heal(int hp) -> void{
        health+=hp;
        if(health > maxHealth) health = maxHealth;
    }
    auto creature::heal() -> void{
        heal(maxHealth-health);
    }


    auto creature::addEXP(int earnedEXP) -> void {
        currentExp+=earnedEXP;
    }

    /**
     *
     * @return returns true if the creature can be improved
     */
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

    /**
     * automatically upgrades the creature
     */
    auto creature::levelUp(int levels) -> void {
        currentLevel += levels;

        maxHealth += (levels*10);
        health = maxHealth;
        maxStrength += (levels*5);
        strength = maxStrength;
        skill += (levels*2);


        expLimit += (levels*25);
    }


}