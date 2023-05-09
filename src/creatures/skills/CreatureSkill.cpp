#include <sstream>
#include <utility>

#include "../../../includes/creatures/skills/CreatureSkill.h"

using namespace MagicalForestFights::Creatures::Skills;

namespace MagicalForestFights::Creatures {
    double CreatureSkill::get_skill_activation_percentage() const {
        return skill_activation_percentage / 100.0;
    }

    CreatureSkill::CreatureSkill(std::string skill_name, SkillType skill_type, std::string skill_desc,
                                 double activation_percentage,
                                 float skill_factor) :
            skill_name(std::move(skill_name)),
            skill_type(skill_type),
            skill_description(std::move(skill_desc)),
            skill_activation_percentage(activation_percentage),
            skill_factor(skill_factor) {}

    std::string CreatureSkill::get_skill_name() const {
        return skill_name;
    }

    std::string CreatureSkill::get_skill_name_with_desc() const {
        std::stringstream ss;
        ss << skill_name << " - " << skill_description;
        return ss.str();
    }
}