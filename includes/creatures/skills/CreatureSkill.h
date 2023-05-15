#include <string>
#include <string_view>
#include <functional>
#include <utility>

#ifndef MAGICALFOESTFIGHTS_CREATURESKILL_H
#define MAGICALFOESTFIGHTS_CREATURESKILL_H

namespace MagicalForestFights::Creatures::Skills {
    enum SkillType { ATTACK, DEFENSE };

    class CreatureSkill {
    private:
        typedef std::function<float(float)> CurriedSkillFactorFn;
        typedef std::function<float(float, float)> SkillFactorFnType;

        template<typename Function, typename... Arguments>
        auto _curry(Function func, Arguments... args) const {
            return [=](auto... rest) {
                return func(args..., rest...);
            };
        }

    public:
        [[nodiscard]] std::string get_skill_name() const;

        [[nodiscard]] std::string get_skill_name_with_desc() const;

        [[nodiscard]] double get_skill_activation_percentage() const;

        [[nodiscard]] CurriedSkillFactorFn UseFn(SkillFactorFnType fn) const {
            return _curry(std::move(fn), this->skill_factor);
        }

        SkillFactorFnType SKILL_FACTOR_FN = [](float x, float y) -> float {
            return x * y;
        };

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
