//
// Created by Stefano Tondo on 23.02.23.
//

//#include "../../../includes/creatures/stats/CreatureStat.h"
//
//using namespace MagicalForestFights::Creatures::Stats;

//namespace MagicalForestFights::Creatures {
//    template <typename T>
//    T CreatureStat<T>::get_current_value() const {
//        return static_cast<T>(current_value);
//    }

//    template <typename T>
//    CreatureStat<T>::CreatureStat(int min, int max) : min_value(min), max_value(max),
//                                                   current_value(generate_init_stat(min, max)) {}

//    template <typename T>
//    T CreatureStat<T>::generate_init_stat(int min, int max) {
//        std::random_device rd; // obtain a generateLuckThreshold number from hardware
//        static std::mt19937 gen(rd()); // seed the generator
//        std::uniform_int_distribution<> distr(min, max); // define the range
////        return distr(gen);
//        return static_cast<T>(distr(gen));
//    }
//}

//int CreatureStat::get_max_value() {
//    return max_value;
//}
