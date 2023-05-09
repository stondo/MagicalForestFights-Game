#ifndef MAGICALFOESTFIGHTS_HEALTH_H
#define MAGICALFOESTFIGHTS_HEALTH_H


#include "CreatureStat.h"

namespace MagicalForestFights::Creatures::Stats {
    template <typename T = float>
    class Health : public CreatureStat<T> {
        using base_t = CreatureStat<T>;
    public:
        Health(int min, int max) : CreatureStat<T>() {
            base_t::min_value = min;
            base_t::max_value = max;
            base_t::current_value = generate_init_stat(min, max);
        }

        Health() = default;

        void SetHealth(T value) {
            base_t::current_value = value;
//            this->current_value = value;
        };

        T get_current_value() override {
            return static_cast<T>(base_t::current_value);
        }

        T generate_init_stat(int min, int max) override {
            std::random_device rd; // obtain a generateLuckThreshold number from hardware
            static std::mt19937 gen(rd()); // seed the generator
            std::uniform_int_distribution<> distr(min, max); // define the range
            return static_cast<T>(distr(gen));
        }

    };
}
#endif //MAGICALFOESTFIGHTS_HEALTH_H
