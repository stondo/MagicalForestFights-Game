#include <string_view>
#include <utility>

#include "../../../includes/creatures/skills/CreatureSkill.h"

using namespace MagicalForestFights::Creatures::Skills;

namespace MagicalForestFights::Creatures {
    double CreatureSkill::get_skill_activation_percentage() const {
        return skill_activation_percentage / 100.0;
    }

    CreatureSkill::CreatureSkill(std::string skill_name, SkillType skill_type, std::string skill_desc,
                                 double percentage) :
            skill_name(std::move(skill_name)),
            skill_type(std::move(skill_type)),
            skill_description(std::move(skill_desc)),
            skill_activation_percentage(percentage) {}

    std::string CreatureSkill::get_skill_name() const {
        return skill_name;
    }

    std::string CreatureSkill::get_skill_name_with_desc() {
        return skill_name + " - " + skill_description;
    }
}