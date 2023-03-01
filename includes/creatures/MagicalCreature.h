#ifndef MAGICALFOESTFIGHTS_MAGICALCREATURE_H
#define MAGICALFOESTFIGHTS_MAGICALCREATURE_H


#include <utility>
#include <tuple>
#include <string_view>
#include <vector>

#include "stats/CreatureStat.h"
#include "stats/Health.h"
#include "stats/Strength.h"
#include "stats/Defence.h"
#include "stats/Luck.h"
#include "stats/Speed.h"
#include "skills/CreatureSkill.h"
#include "MagicalCreatureState.h"
#include "../constants.h"

using namespace MagicalForestFights::Creatures::Stats;
using namespace MagicalForestFights::Creatures::Skills;

namespace MagicalForestFights::Creatures {
    class MagicalCreature {
    private:
        std::string name;
        Health<float> health;
        Strength<int> strength;
        Defence<int> defence;
        Speed<int> speed;
        Luck<double> luck;
        std::vector<CreatureSkill> attacking_skills;
        std::vector<CreatureSkill> defending_skills;
        MagicalCreatureState current_state = Alive();
        double current_luck_threshold;

    public:
        std::string GetName();
        float GetHealth();
        int GetStrength();
        int GetDefence() ;
        int GetSpeed();
        float GetLuck();
        [[nodiscard]] double GetCurrentLuckthreshold() const;
        [[nodiscard]] MagicalCreatureState GetCurrentState() const;
        [[nodiscard]] const std::vector<CreatureSkill> & GetAttackingSkills() const;
        [[nodiscard]] const std::vector<CreatureSkill> & GetDefendingSkills() const;

        void UpdateDefenderHealthAndState(float damage_endured);

        bool IsLuckyEnoughToDodgeAttack();
        bool IsLuckyEnoughToTriggerSkill(double skill_activation_percentage);

        void SetHealth(Health<float> h);
        void SetStrength(Strength<int> s);
        void SetDefence(Defence<int> d);
        void SetSpeed(Speed<int> s);
        void SetLuck(Luck<double> l);
        void SetCurrentState(MagicalCreatureState state);
        void SetCurrentLuckthreshold(double luck_threshold);

        void PrintSkills();
        void PrintInitStats();

        MagicalCreature(std::string name, std::tuple<int, int> health_tpl, std::tuple<int, int> strength_tpl,
                        std::tuple<int, int> defence_tpl, std::tuple<int, int> speed_tpl, std::tuple<int, int> luck_tpl,
                        std::vector<CreatureSkill> attacking_skills, std::vector<CreatureSkill> defending_skills);
    };
}



#endif //MAGICALFOESTFIGHTS_MAGICALCREATURE_H
