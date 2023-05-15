#include <tuple>
#include <iostream>
#include <sstream>
#include <utility>
#include "spdlog/spdlog.h"

#include "../../includes/creatures/MagicalCreature.h"

using namespace std;

namespace MagicalForestFights::Creatures {

    std::string MagicalCreature::GetName() {
        return name;
    }

    float MagicalCreature::GetHealth() {
        return health.get_current_value();
    }

    int MagicalCreature::GetStrength() {
        return strength.get_current_value();
    }

    int MagicalCreature::GetDefence() {
        return defence.get_current_value();
    }

    int MagicalCreature::GetSpeed() {
        return speed.get_current_value();
    }

    double MagicalCreature::GetLuck() {
        return luck.get_current_value();
    }

    void MagicalCreature::PrintInitStats() {
        spdlog::info("{} is {}:", name, currentState.get_value());
        spdlog::info("Health: {}", health.get_current_value());
        spdlog::info("Strength: {}", strength.get_current_value());
        spdlog::info("Defence: {}", defence.get_current_value());
        spdlog::info("Speed: {}", speed.get_current_value());
        spdlog::info("Luck: {}%", luck.get_current_value());

        PrintSkills();
    }

    void MagicalCreature::PrintSkills() {
        auto loop_skills_and_print = [](const string& skill_type, const vector<CreatureSkill>& skills) -> void {
            spdlog::info("{} Skills: ", skill_type);
            for (const auto& skill: skills)
                spdlog::info(skill.get_skill_name_with_desc());
        };

        if (!attackingSkills.empty())
            loop_skills_and_print(ATTACKING, attackingSkills);
        else
            spdlog::info("Wild Beast doesn't have any special attacking skill.");

        if (!defendingSkills.empty())
            loop_skills_and_print(DEFENDING, defendingSkills);
        else
            spdlog::info("Wild Beast doesn't have any special defending skill.");
    }

    void MagicalCreature::SetCurrentState(MagicalCreatureState state) {
        currentState = std::move(state);
    }

    void MagicalCreature::UpdateDefenderHealthAndState(float damage_endured) {
        float health_left = health.get_current_value() - damage_endured;

        if (health_left <= 0) {
            health.SetHealth(0);
            this->SetCurrentState(Dead());
        } else
            health.SetHealth(health_left);
    }

    void MagicalCreature::SetHealth(Health<float> h) {
        health = std::move(h);
    }

    void MagicalCreature::SetStrength(Strength<int> s) {
        strength = std::move(s);
    }
    void MagicalCreature::SetDefence(Defence<int> d) {
        defence = std::move(d);
    }

    void MagicalCreature::SetSpeed(Speed<int> s) {
        speed = std::move(s);
    }

    void MagicalCreature::SetLuck(Luck<double> l) {
        luck = std::move(l);
    }

    MagicalCreatureState MagicalCreature::GetCurrentState() const {
        return currentState;
    }

    const std::vector<CreatureSkill> &MagicalCreature::GetAttackingSkills() const {
        return attackingSkills;
    }

    const std::vector<CreatureSkill> &MagicalCreature::GetDefendingSkills() const {
        return defendingSkills;
    }

    void MagicalCreature::SetCurrentLuckthreshold(double luck_threshold) {
        currentLuckThreshold = luck_threshold;
    }

    bool MagicalCreature::IsLuckyEnoughToDodgeAttack() {
        return this->GetLuck() >= this->currentLuckThreshold;
    }

    bool MagicalCreature::IsLuckyEnoughToTriggerSkill(double skill_activation_percentage) {
            return this->currentLuckThreshold <= skill_activation_percentage;
    }

    double MagicalCreature::GetCurrentLuckthreshold() const {
        return this->currentLuckThreshold;
    }

    MagicalCreature::MagicalCreature(string name, tuple<int, int> health_tpl, tuple<int, int> strength_tpl,
                                     tuple<int, int> defence_tpl, tuple<int, int> speed_tpl, tuple<int, int> luck_tpl,
                                     vector<CreatureSkill> attacking_skills, vector<CreatureSkill> defending_skills) : name(std::move(name)),
                                                                          health(Health<float>(
                                                                                       get<0>(health_tpl),
                                                                                       get<1>(health_tpl))),
                                                                          strength(Strength<int>(
                                                                                       get<0>(strength_tpl),
                                                                                       get<1>(strength_tpl))),
                                                                          defence(Defence<int>(
                                                                                       get<0>(defence_tpl),
                                                                                       get<1>(defence_tpl))),
                                                                          speed(Speed<int>(
                                                                                       get<0>(speed_tpl),
                                                                                       get<1>(speed_tpl))),
                                                                          luck(Luck<double>(
                                                                                       get<0>(luck_tpl),
                                                                                       get<1>(luck_tpl))),
                                                                          attackingSkills(std::move(
                                                                                       attacking_skills)),
                                                                          defendingSkills(std::move(
                                                                                       defending_skills)) {
    }

    MagicalCreature::MagicalCreature(const MagicalCreature &rhs) : name(rhs.name),
                                                                   health(rhs.health),
                                                                   strength(rhs.strength),
                                                                   defence(rhs.defence),
                                                                   speed(rhs.speed),
                                                                   luck(rhs.luck),
                                                                   attackingSkills(rhs.attackingSkills),
                                                                   defendingSkills(rhs.defendingSkills) {
        currentLuckThreshold = rhs.currentLuckThreshold;
        spdlog::info("Copy constructor called");
    }

    MagicalCreature &MagicalCreature::operator=(const MagicalCreature &rhs) {
        spdlog::info("Copy assignment operator called");

        name = rhs.name;
        health = rhs.health;
        strength = rhs.strength;
        defence = rhs.defence;
        speed = rhs.speed;
        luck = rhs.luck;
        currentState = rhs.currentState;
        attackingSkills = rhs.attackingSkills;
        defendingSkills = rhs.defendingSkills;
        currentLuckThreshold = rhs.currentLuckThreshold;
        return *this;
    }

    MagicalCreature::MagicalCreature(MagicalCreature &&rhs) noexcept {
        spdlog::info("Move constructor called");

        name = std::move(rhs.name);
        health = std::move(rhs.health);
        strength = std::move(rhs.strength);
        defence = std::move(rhs.defence);
        speed = std::move(rhs.speed);
        luck = std::move(rhs.luck);
        currentState = std::move(rhs.currentState);
        attackingSkills = std::move(rhs.attackingSkills);
        defendingSkills = std::move(rhs.defendingSkills);
        currentLuckThreshold = rhs.currentLuckThreshold;
    }

    MagicalCreature &MagicalCreature::operator=(MagicalCreature &&rhs) noexcept {
        spdlog::info("Move assignment operator called");

        name = std::move(rhs.name);
        health = std::move(rhs.health);
        strength = std::move(rhs.strength);
        defence = std::move(rhs.defence);
        speed = std::move(rhs.speed);
        luck = std::move(rhs.luck);
        currentState = std::move(rhs.currentState);
        attackingSkills = std::move(rhs.attackingSkills);
        defendingSkills = std::move(rhs.defendingSkills);
        currentLuckThreshold = rhs.currentLuckThreshold;
        return *this;
    }


}