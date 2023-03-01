#ifndef MAGICALFOESTFIGHTS_LUCK_H
#define MAGICALFOESTFIGHTS_LUCK_H


#include "CreatureStat.h"

namespace MagicalForestFights::Creatures::Stats {
    template <typename T = double>
    class Luck : public CreatureStat<T> {
        using base_t = CreatureStat<T>;
    public:
        Luck(int min, int max) : CreatureStat<T>() {
//            this->current_value /= 100.0;
            base_t::min_value = min;
            base_t::max_value = max;
            base_t::current_value = generate_init_stat(min, max) / 100.0;
        }

        T get_current_value() override {
            return static_cast<T>(base_t::current_value);
        }

        T generate_init_stat(int min, int max) override {
            std::random_device rd; // obtain a generate_luck_threshold number from hardware
            static std::mt19937 gen(rd()); // seed the generator
            std::uniform_int_distribution<> distr(min, max); // define the range
            return static_cast<T>(distr(gen));
        }
    };
}

#endif //MAGICALFOESTFIGHTS_LUCK_H
