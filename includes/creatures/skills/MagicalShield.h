#ifndef MAGICALFORESTFIGHTS_MAGICALSHIELD_H
#define MAGICALFORESTFIGHTS_MAGICALSHIELD_H

namespace MagicalForestFights::Creatures::Skills {
    class MagicalShield : public CreatureSkill {
    public:
        MagicalShield() : CreatureSkill{"Magical Shield",
                                        SkillType::DEFENSE,
                                        "Takes only half of the usual damage when an enemy attacks, there's a 20% chance he'll use this skill every time he defends.",
                                        20, 0.5f} {}

        MagicalShield(const std::string& skillName, SkillType skillType, const std::string& skillDesc, double activationPercentage, float skillFactor)
                : CreatureSkill{skillName,
                                skillType,
                                skillDesc,
                                activationPercentage,
                                skillFactor} {}
    };
}
#endif //MAGICALFORESTFIGHTS_MAGICALSHIELD_H
