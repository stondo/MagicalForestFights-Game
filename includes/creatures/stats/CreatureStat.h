#include <random>

#ifndef MAGICALFOESTFIGHTS_CREATURESTATS_H
#define MAGICALFOESTFIGHTS_CREATURESTATS_H

namespace MagicalForestFights::Creatures::Stats {
    template <typename T>
    class CreatureStat {
    public:
        CreatureStat() = default;               // <== was missing impl, set to default
        virtual ~CreatureStat() = default;      // classes with virtual methods must have virtual destructor
        virtual T get_current_value() = 0;
        virtual T generate_init_stat(int min, int max) = 0;

        T Get_min_value() {
            return min_value;
        };
        T Get_max_value() {
            return max_value;
        };

    protected:
        int min_value{0};
        int max_value{0};
        T current_value;
    };
}

#endif //MAGICALFOESTFIGHTS_CREATURESTATS_H
