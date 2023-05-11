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
        LUCK,
        SKILLS
    };

    enum PropertyRange {
        MIN = 0,
        MAX
    };

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

        void ProcessFight(int current_turn = 1, int max_turns = 20);

        FightManager();
        FightManager(const std::unique_ptr<MagicalCreature> &hero, const std::unique_ptr<MagicalCreature> &beast);

        void swapAttDef();

        void SetFleeingState(int max_turns);

        bool AnyMagicalCreatureDied();

    private:
        std::unique_ptr<MagicalCreature> attacker{};
        std::unique_ptr<MagicalCreature> defender{};
        float _damage = 0.0;

        static bool hasLuck(double luck_threshold, float creature_luck);
        static double generateLuckThreshold();
        void setAttackerAndDefender(const std::unique_ptr<MagicalCreature> &hero, const std::unique_ptr<MagicalCreature> &beast);
        [[nodiscard]] float calculateDamage() const;

        template <typename... Strings>
        auto concatenate(Strings... strings);

        void readInitDataAndCreateMagicalCreatures();
    };
}


#endif //MAGICALFOESTFIGHTS_FIGHTMANAGER_H
