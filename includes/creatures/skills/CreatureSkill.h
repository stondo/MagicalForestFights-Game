#include <string>
#include <string_view>
#include <functional>
#include "SkillType.h"

#ifndef MAGICALFOESTFIGHTS_CREATURESKILL_H
#define MAGICALFOESTFIGHTS_CREATURESKILL_H

namespace MagicalForestFights::Creatures::Skills {
    template<typename Function, typename... Arguments>
    auto curry(Function func, Arguments... args) {
        return [=](auto... rest) {
            return func(args..., rest...);
        };
    }

    auto SKILL_FACTOR_FN = [](auto x, auto y) {
        return x * y;
    };

    typedef std::function<float(float)> CurriedSkillFactorFn;
    typedef std::function<float(float, float)> SkillFactorFnType;

    class CreatureSkill {
    public:
        [[nodiscard]] std::string get_skill_name() const;

        std::string get_skill_name_with_desc();

        [[nodiscard]] double get_skill_activation_percentage() const;

        [[nodiscard]] CurriedSkillFactorFn UseFn(SkillFactorFnType fn) const {
            return curry(fn, this->skill_factor);
        }

        std::string skill_name;
        SkillType skill_type;
        std::string skill_description;
        double skill_activation_percentage{0.0};
        float skill_factor{1.0f};

        CreatureSkill(std::string skill_name, SkillType skill_type, std::string skill_desc,
                      double activation_percentage,
                      float skill_factor);
    };
}

#endif //MAGICALFOESTFIGHTS_CREATURESKILL_H
