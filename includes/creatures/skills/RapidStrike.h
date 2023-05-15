#ifndef MAGICALFORESTFIGHTS_RAPIDSTRIKE_H
#define MAGICALFORESTFIGHTS_RAPIDSTRIKE_H

namespace MagicalForestFights::Creatures::Skills {

    class RapidStrike : public CreatureSkill {
    public:
        RapidStrike()
                : CreatureSkill{"Rapid Strike",
                                SkillType::ATTACK,
                                "Strike twice while it's his turn to attack, there's a 10% chance he'll use this skill every time he attacks.",
                                10.0,
                                2.0f} {}

        RapidStrike(const std::string& skillName, SkillType skillType, const std::string& skillDesc, double activationPercentage, float skillFactor)
                : CreatureSkill{skillName,
                                skillType,
                                skillDesc,
                                activationPercentage,
                                skillFactor} {}
    };

}

#endif //MAGICALFORESTFIGHTS_RAPIDSTRIKE_H
