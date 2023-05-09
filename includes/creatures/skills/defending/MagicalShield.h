//
// Created by Stefano Tondo on 06.05.23.
//

#ifndef MAGICALFORESTFIGHTS_MAGICALSHIELD_H
#define MAGICALFORESTFIGHTS_MAGICALSHIELD_H

#include "../CreatureSkill.h"

namespace MagicalForestFights::Creatures::Skills {
    class MagicalShield : public CreatureSkill {
    public:
        MagicalShield() : CreatureSkill{"Magical Shield",
                                        SkillType::DEFENSE,
                                        "Takes only half of the usual damage when an enemy attacks, there's a 20% chance he'll use this skill every time he defends.",
                                        20, 0.5f} {}
    };
}
#endif //MAGICALFORESTFIGHTS_MAGICALSHIELD_H
