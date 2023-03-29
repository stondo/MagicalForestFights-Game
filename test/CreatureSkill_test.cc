#include <gtest/gtest.h>
#include "../includes/creatures/skills/CreatureSkill.h"

using namespace MagicalForestFights::Creatures::Skills;
using ::testing::DoubleLE;

class CreatureSkillTest : public ::testing::Test {
protected:
    CreatureSkill rapid_strike = CreatureSkill(
            "Rapid Strike",
            Attack(),
            "Strike twice while it's his turn to attack, there's a 10% chance he'll use this skill every time he attacks",
            10, 2.0f);
    CreatureSkill magical_shield = CreatureSkill(
            "Magical Shield",
            Defense(),
            "Takes only half of the usual damage when an enemy attacks, there's a 20% chance he'll use this skill every time he defends.",
            20, 0.5f);

};

TEST_F(CreatureSkillTest, SkillsActivationPercentage) {
    double rapid_strike_expected_percentage = 0.1;
    double magical_shield_expected_percentage = 0.2;

    EXPECT_PRED_FORMAT2(DoubleLE, rapid_strike_expected_percentage, rapid_strike.get_skill_activation_percentage());
    EXPECT_PRED_FORMAT2(DoubleLE, magical_shield_expected_percentage, magical_shield.get_skill_activation_percentage());
}

TEST_F(CreatureSkillTest, SkillsType) {
    EXPECT_EQ(rapid_strike.skill_type.get_value(), ATTACK);
    EXPECT_EQ(magical_shield.skill_type.get_value(), DEFENSE);
}
