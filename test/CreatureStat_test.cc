#include <gtest/gtest.h>
#include "../includes/creatures/stats/CreatureStat.h"
#include "../includes/creatures/stats/Health.h"
#include "../includes/creatures/stats/Strength.h"
#include "../includes/creatures/stats/Defence.h"
#include "../includes/creatures/stats/Speed.h"
#include "../includes/creatures/stats/Luck.h"

using namespace MagicalForestFights::Creatures::Stats;
using ::testing::DoubleLE;

class CreatureStatsTest : public ::testing::Test {
protected:
    int zero = 0;
    int one_hundred = 100;
    int min = 50;
    int max = 99;
    double never_lucky = 0.0;
    double always_lucky = 1.0;

    Health<float> health = Health<float>(min, max);
    Strength<int> strength = Strength<int>(one_hundred, one_hundred);
    Defence<int> defence = Defence<int>(min, max);
    Speed<int> speed = Speed<int>(min, max);
    Luck<double> luck = Luck<double>(zero, max);

    Luck<double> unlucky = Luck<double>(zero, zero);
    Luck<double> lucky = Luck<double>(one_hundred, one_hundred);
};

TEST_F(CreatureStatsTest, StatsMinMax) {
    EXPECT_EQ(health.Get_min_value(), min);
    EXPECT_EQ(health.Get_max_value(), max);

    EXPECT_EQ(defence.Get_min_value(), min);
    EXPECT_EQ(defence.Get_max_value(), max);

    EXPECT_EQ(speed.Get_min_value(), min);
    EXPECT_EQ(speed.Get_max_value(), max);

    EXPECT_EQ(strength.Get_min_value(), one_hundred);
    EXPECT_EQ(strength.Get_max_value(), one_hundred);
    EXPECT_EQ(strength.Get_min_value(), strength.Get_max_value());

    EXPECT_EQ(luck.Get_min_value(), zero);
    EXPECT_EQ(luck.Get_max_value(), max);
}

TEST_F(CreatureStatsTest, StatsCurrentValue) {
    EXPECT_EQ(strength.get_current_value(), one_hundred);
}

TEST_F(CreatureStatsTest, LuckPercentage) {
    EXPECT_PRED_FORMAT2(DoubleLE, never_lucky, unlucky.get_current_value());
    EXPECT_PRED_FORMAT2(DoubleLE, always_lucky, lucky.get_current_value());
}
