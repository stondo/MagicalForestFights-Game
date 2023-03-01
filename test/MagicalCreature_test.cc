#include <gtest/gtest.h>
#include "../includes/creatures/MagicalCreature.h"
#include "../includes/creatures/FightManager.h"

using namespace MagicalForestFights::Creatures;
using ::testing::DoubleLE;
using ::testing::FloatLE;

class MagicalCreatureTest : public ::testing::Test {
protected:
    std::string hero_name = "Natelus";
    std::tuple<int, int> hero_health_tpl = std::tuple<int, int>(70, 100);
    std::tuple<int, int> hero_strength_tpl = std::tuple<int, int>(70, 80);
    std::tuple<int, int> hero_defence_tpl = std::tuple<int, int>(45, 55);
    std::tuple<int, int> hero_speed_tpl = std::tuple<int, int>(40, 50);
    std::tuple<int, int> hero_luck_tpl = std::tuple<int, int>(10, 30);

    MagicalCreature natelus = MagicalCreature(hero_name, hero_health_tpl, hero_strength_tpl,
                                                   hero_defence_tpl,hero_speed_tpl, hero_luck_tpl,
                                  std::vector<CreatureSkill>{CreatureSkill(
                                          "Rapid Strike",
                                          Attack(),
                                          "Strike twice while it's his turn to attack, there's a 10% chance he'll use this skill every time he attacks",
                                          10)},
                                  std::vector<CreatureSkill>{CreatureSkill(
                                          "Magical Shield",
                                          Defense(),
                                          "Takes only half of the usual damage when an enemy attacks, there's a 20% chance he'll use this skill every time he defends.",
                                          20)});

    std::string wildbeast_name = "Wild Beast";
    std::tuple<int, int> wildbeast_health_tpl = std::tuple<int, int>(60, 90);
    std::tuple<int, int> wildbeast_strength_tpl = std::tuple<int, int>(60, 90);
    std::tuple<int, int> wildbeast_defence_tpl = std::tuple<int, int>(40, 60);
    std::tuple<int, int> wildbeast_speed_tpl = std::tuple<int, int>(40, 60);
    std::tuple<int, int> wildbeast_luck_tpl = std::tuple<int, int>(25, 40);

    MagicalCreature wild_beast = MagicalCreature(wildbeast_name, wildbeast_health_tpl, wildbeast_strength_tpl,
                                     wildbeast_defence_tpl, wildbeast_speed_tpl, wildbeast_luck_tpl,
                                     std::vector<CreatureSkill>{},
                                     std::vector<CreatureSkill>{});

    MagicalCreature god_natelus = MagicalCreature("NatelusGod", std::tuple<int, int>(100, 100),
                                  std::tuple<int, int>(100, 100),
                                  std::tuple<int, int>(100, 100), std::tuple<int, int>(100, 100),
                                  std::tuple<int, int>(100, 100),
                                  std::vector<CreatureSkill>{CreatureSkill(
                                          "Rapid Strike",
                                          Attack(),
                                          "Strike twice while it's his turn to attack, there's a 10% chance he'll use this skill every time he attacks",
                                          100)},
                                  std::vector<CreatureSkill>{CreatureSkill(
                                          "Magical Shield",
                                          Defense(),
                                          "Takes only half of the usual damage when an enemy attacks, there's a 20% chance he'll use this skill every time he defends.",
                                          100)});

    MagicalCreature god_natelus_no_skill = MagicalCreature("NatelusGod", std::tuple<int, int>(10, 100),
                                                  std::tuple<int, int>(100, 100),
                                                  std::tuple<int, int>(100, 100), std::tuple<int, int>(100, 100),
                                                  std::tuple<int, int>(100, 100),
                                                           std::vector<CreatureSkill>{},
                                                           std::vector<CreatureSkill>{});

    MagicalCreature weak_wild_beast = MagicalCreature("Weak Wild Beast", std::tuple<int, int>(50, 50),
                                     std::tuple<int, int>(50, 50),
                                     std::tuple<int, int>(50, 50), std::tuple<int, int>(50, 50),
                                     std::tuple<int, int>(0, 0),
                                     std::vector<CreatureSkill>{},
                                     std::vector<CreatureSkill>{});
};

TEST_F(MagicalCreatureTest, MagicalCreaturesAreAliveWhenInit) {
    std::string v1 = natelus.GetCurrentState().get_value();
    std::string v2 = Alive().get_value();
    std::string v3 = Dead().get_value();

    EXPECT_STREQ(v1.c_str(), v2.c_str());
    EXPECT_STRNE(v1.c_str(), v3.c_str());
    EXPECT_STRNE(v2.c_str(), v3.c_str());
}

TEST_F(MagicalCreatureTest, MagicalCreaturesStatsAreInExpectedRanges) {
    EXPECT_GE(natelus.GetHealth(), get<0>(hero_health_tpl));
    EXPECT_LE(natelus.GetHealth(), get<1>(hero_health_tpl));
    EXPECT_GE(natelus.GetStrength(), get<0>(hero_strength_tpl));
    EXPECT_LE(natelus.GetStrength(), get<1>(hero_strength_tpl));
    EXPECT_GE(natelus.GetDefence(), get<0>(hero_defence_tpl));
    EXPECT_LE(natelus.GetDefence(), get<1>(hero_defence_tpl));
    EXPECT_GE(natelus.GetSpeed(), get<0>(hero_speed_tpl));
    EXPECT_LE(natelus.GetSpeed(), get<1>(hero_speed_tpl));

    EXPECT_GE(wild_beast.GetHealth(), get<0>(wildbeast_health_tpl));
    EXPECT_LE(wild_beast.GetHealth(), get<1>(wildbeast_health_tpl));
    EXPECT_GE(wild_beast.GetStrength(), get<0>(wildbeast_strength_tpl));
    EXPECT_LE(wild_beast.GetStrength(), get<1>(wildbeast_strength_tpl));
    EXPECT_GE(wild_beast.GetDefence(), get<0>(wildbeast_defence_tpl));
    EXPECT_LE(wild_beast.GetDefence(), get<1>(wildbeast_defence_tpl));
    EXPECT_GE(wild_beast.GetSpeed(), get<0>(wildbeast_speed_tpl));
    EXPECT_LE(wild_beast.GetSpeed(), get<1>(wildbeast_speed_tpl));
}

TEST_F(MagicalCreatureTest, VerifyCorrectNumOfSkills) {
    EXPECT_EQ(natelus.GetAttackingSkills().size(), 1);
    EXPECT_EQ(natelus.GetDefendingSkills().size(), 1);
    EXPECT_EQ(wild_beast.GetDefendingSkills().size(), 0);
}

TEST_F(MagicalCreatureTest, VerifyPercentageActivationOfSkills) {
    double rapid_strike_expected_activation_percentage = 0.1;
    double magic_shield_expected_activation_percentage = 0.2;

    EXPECT_PRED_FORMAT2(DoubleLE, natelus.GetAttackingSkills().at(0).get_skill_activation_percentage(),
                        rapid_strike_expected_activation_percentage);
    EXPECT_PRED_FORMAT2(DoubleLE, natelus.GetDefendingSkills().at(0).get_skill_activation_percentage(),
                        magic_shield_expected_activation_percentage);
}

TEST_F(MagicalCreatureTest, VerifyFightAndDamageNoLuck) {
    auto fm = FightManager(&god_natelus_no_skill, &weak_wild_beast);
    auto [damage,
                  attacker_used_skill_output,
                  defender_dodged_output,
                  defender_used_skill_output] = fm.ProcessFight();
    EXPECT_EQ(damage, 50);
}

TEST_F(MagicalCreatureTest, VerifyFightAndDamageRapidStrike) {
    auto fm = FightManager(&god_natelus, &weak_wild_beast);
    auto [damage,
                  attacker_used_skill_output,
                  defender_dodged_output,
                  defender_used_skill_output] = fm.ProcessFight();
    EXPECT_EQ(damage, 100);
}

TEST_F(MagicalCreatureTest, VerifyDodgeIfLucky) {
    god_natelus.SetSpeed(Speed<int>(49, 49));
    EXPECT_EQ(god_natelus.GetSpeed(), 49);

    auto fm = FightManager(&god_natelus, &weak_wild_beast);
    auto [damage,
                  attacker_used_skill_output,
                  defender_dodged_output,
                  defender_used_skill_output] = fm.ProcessFight();
    EXPECT_EQ(damage, 0);
}

TEST_F(MagicalCreatureTest, VerifyMagicalShield) {
    god_natelus.SetSpeed(Speed<int>(49, 49));
    EXPECT_EQ(god_natelus.GetSpeed(), 49);

    god_natelus.SetLuck(Luck<double>(0, 0));
    EXPECT_EQ(god_natelus.GetLuck(), 0);

    god_natelus.SetDefence(Defence<int>(25, 25));
    EXPECT_EQ(god_natelus.GetDefence(), 25);

    auto fm = FightManager(&god_natelus, &weak_wild_beast);
    auto [damage,
                  attacker_used_skill_output,
                  defender_dodged_output,
                  defender_used_skill_output] = fm.ProcessFight();

    EXPECT_EQ(damage, 12.5f);
    EXPECT_EQ(god_natelus.GetHealth(), 87.5f);
    EXPECT_EQ(fm.defender->GetHealth(), 87.5f);
}

TEST_F(MagicalCreatureTest, VerifyStateAfterMortalBlow) {
    auto fm = FightManager(&god_natelus, &weak_wild_beast);
    auto [damage,
                  attacker_used_skill_output,
                  defender_dodged_output,
                  defender_used_skill_output] = fm.ProcessFight();

    EXPECT_EQ(damage, 100);
    EXPECT_STREQ(weak_wild_beast.GetCurrentState().get_value().c_str(), Dead().get_value().c_str());
}
