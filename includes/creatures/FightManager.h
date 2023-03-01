#ifndef MAGICALFOESTFIGHTS_FIGHTMANAGER_H
#define MAGICALFOESTFIGHTS_FIGHTMANAGER_H

#include <random>
#include <utility>
#include <tuple>
#include "MagicalCreature.h"

using namespace std;

namespace MagicalForestFights::Creatures {
    class FightManager {
    public:
        MagicalForestFights::Creatures::MagicalCreature *attacker;
        MagicalForestFights::Creatures::MagicalCreature *defender;

        bool hasLuck(double luck_threshold, float creature_luck);
        static double generate_luck_threshold();

        std::tuple<float, std::string, std::string, std::string> ProcessFight();

        void PrintFightInfo(float damage, const std::string& attacker_used_skill_output, const std::string& defender_dodged_output,
                            const std::string& defender_used_skill_output, int current_turn, int max_turns);

        FightManager(MagicalCreature *hero, MagicalCreature *beast);

    private:
        void set_attacker_and_defender(MagicalCreature *hero, MagicalCreature *beast);
    };
}


#endif //MAGICALFOESTFIGHTS_FIGHTMANAGER_H
