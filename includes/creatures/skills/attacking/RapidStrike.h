//
// Created by Stefano Tondo on 06.05.23.
//

#ifndef MAGICALFORESTFIGHTS_RAPIDSTRIKE_H
#define MAGICALFORESTFIGHTS_RAPIDSTRIKE_H

#include "../CreatureSkill.h"

namespace MagicalForestFights::Creatures::Skills {

    class RapidStrike : public CreatureSkill {
    public:
        RapidStrike()
                : CreatureSkill{"Rapid Strike",
                                SkillType::ATTACK,
                                "Strike twice while it's his turn to attack, there's a 10% chance he'll use this skill every time he attacks.",
                                10.0,
                                2.0f} {}
    };

}

#endif //MAGICALFORESTFIGHTS_RAPIDSTRIKE_H
