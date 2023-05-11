#include <gtest/gtest.h>
#include "../includes/creatures/skills/CreatureSkill.h"
#include "../includes/creatures/skills/RapidStrike.h"
#include "../includes/creatures/skills/MagicalShield.h"

using namespace MagicalForestFights::Creatures::Skills;
using ::testing::DoubleLE;

class CreatureSkillTest : public ::testing::Test {
protected:
    CreatureSkill rapid_strike = RapidStrike();
    CreatureSkill magical_shield = MagicalShield();

};

TEST_F(CreatureSkillTest, SkillsActivationPercentage) {
    double rapid_strike_expected_percentage = 0.1;
    double magical_shield_expected_percentage = 0.2;

    EXPECT_PRED_FORMAT2(DoubleLE, rapid_strike_expected_percentage, rapid_strike.get_skill_activation_percentage());
    EXPECT_PRED_FORMAT2(DoubleLE, magical_shield_expected_percentage, magical_shield.get_skill_activation_percentage());
}

TEST_F(CreatureSkillTest, SkillsType) {
    EXPECT_EQ(rapid_strike.skill_type, ATTACK);
    EXPECT_EQ(magical_shield.skill_type, DEFENSE);
}
