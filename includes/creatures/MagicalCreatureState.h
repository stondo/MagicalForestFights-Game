#ifndef MAGICALFOESTFIGHTS_MAGICALCREATURESTATE_H
#define MAGICALFOESTFIGHTS_MAGICALCREATURESTATE_H

#include <string>
#include <string_view>
#include <utility>
#include "../constants.h"

namespace MagicalForestFights::Creatures {
    class MagicalCreatureState {
    public:
        std::string get_value() {
            return value;
        }
        explicit MagicalCreatureState(std::string value) : value(std::move(value)) {}

        bool operator==(const MagicalCreatureState& mcs2) {
            return this->value == mcs2.value;
        }

    protected:
        std::string value;

    };

    class Alive : public MagicalCreatureState {
    public:
        Alive() : MagicalCreatureState(ALIVE) {}

//        bool operator==(const Alive &mcs2) {
//            return this->value == mcs2.value;
//        }

    };

    class Dead : public MagicalCreatureState {
    public:
        Dead() : MagicalCreatureState(DEAD) {}

        bool operator==(const Dead &mcs2) {
            return this->value == mcs2.value;
        }
    };

    class Attacking : public MagicalCreatureState {
    public:
        Attacking() : MagicalCreatureState(ATTACKING) {}
    };

    class Defending : public MagicalCreatureState {
    public:
        Defending() : MagicalCreatureState(DEFENDING) {}
    };

    class Fleeing : public MagicalCreatureState {
    public:
        Fleeing() : MagicalCreatureState(FLEEING) {}
    };
}

#endif //MAGICALFOESTFIGHTS_MAGICALCREATURESTATE_H
