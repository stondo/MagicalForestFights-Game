#include <string>
#include <string_view>
#include "SkillType.h"

#ifndef MAGICALFOESTFIGHTS_CREATURESKILL_H
#define MAGICALFOESTFIGHTS_CREATURESKILL_H

namespace MagicalForestFights::Creatures::Skills {
    class CreatureSkill {
    public:
        std::string get_skill_name() const;

        std::string get_skill_name_with_desc();

        [[nodiscard]] double get_skill_activation_percentage() const;

        std::string skill_name;
        SkillType skill_type;
        std::string skill_description;
        double skill_activation_percentage{0.0};

        CreatureSkill(std::string skill_name, SkillType skill_type, std::string skill_desc, double percentage);
    };
}

#endif //MAGICALFOESTFIGHTS_CREATURESKILL_H
