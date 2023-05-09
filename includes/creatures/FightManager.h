#ifndef MAGICALFOESTFIGHTS_FIGHTMANAGER_H
#define MAGICALFOESTFIGHTS_FIGHTMANAGER_H

#include <random>
#include <utility>
#include <tuple>
#include "MagicalCreature.h"

using namespace std;

namespace MagicalForestFights::Creatures {

    enum CreatureProperty {
        NAME = 0,
        HEALTH,
        STRENGTH,
        DEFENSE,
        SPEED,
        LUCK
    };

    constexpr std::initializer_list<CreatureProperty> CreatureProperties = {NAME, HEALTH, STRENGTH, DEFENSE, SPEED, LUCK};

    enum PropertyRange {
        MIN = 0,
        MAX
    };

    constexpr std::initializer_list<PropertyRange> PropertyRanges = {MIN, MAX};

    enum CreatureSkillProperty {
        SKILL_NAME = 0,
        TYPE,
        DESC,
        ACTIVATION_PERCENTAGE,
        SKILL_FACTOR
    };

    constexpr std::initializer_list<CreatureSkillProperty> CreatureSkillProperties = {SKILL_NAME, TYPE, DESC, ACTIVATION_PERCENTAGE, SKILL_FACTOR};

    class FightManager {
    public:
        [[nodiscard]] float GetDamageDone() const;

        void ProcessFight();

        void PrintFightInfo(int current_turn, int max_turns);

        FightManager();
        FightManager(std::shared_ptr<MagicalCreature> hero, std::shared_ptr<MagicalCreature> beast);

        void swapAttDef();

        void SetFleeingState();

        bool AnyMagicalCreatureDied();

    private:
        std::shared_ptr<MagicalCreature> attacker{};
        std::shared_ptr<MagicalCreature> defender{};

        static bool hasLuck(double luck_threshold, float creature_luck);
        static double generateLuckThreshold();
        void setAttackerAndDefender(const std::shared_ptr<MagicalCreature>& hero, const std::shared_ptr<MagicalCreature>& beast);
        [[nodiscard]] float calculateDamage() const;

        template <typename... Strings>
        auto concatenate(Strings... strings);

        float _damage = 0.0;
        string _attackerUsedSkillOutput;
        string _defenderDodgedOutput;
        string _defenderUsedSkillOutput;

        void readInitDataAndCreateMagicalCreatures();
    };
}


#endif //MAGICALFOESTFIGHTS_FIGHTMANAGER_H
