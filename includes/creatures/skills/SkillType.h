#ifndef MAGICALFOESTFIGHTS_SKILLTYPE_H
#define MAGICALFOESTFIGHTS_SKILLTYPE_H

#include <string>
#include <utility>
#include "../../constants.h"

namespace MagicalForestFights::Creatures::Skills {
    class SkillType {
    public:
        [[nodiscard]] std::string get_value() const {
            return value;
        }
    protected:
        const std::string value;

        explicit SkillType(std::string value) : value(std::move(value)) {}
    };

    class Attack : public SkillType {
    public:
        Attack() : SkillType(ATTACK) {};
    };

    class Defense : public SkillType {
    public:
        Defense() : SkillType(DEFENSE) {};
    };

}


#endif //MAGICALFOESTFIGHTS_SKILLTYPE_H
