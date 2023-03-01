//
// Created by Stefano Tondo on 23.02.23.
//
#include <tuple>
#include <iostream>
#include <string_view>
#include <utility>

#include "../../includes/creatures/MagicalCreature.h"

using namespace std;

namespace MagicalForestFights::Creatures {
    MagicalCreature::MagicalCreature(string name, tuple<int, int> health_tpl,
                                     tuple<int, int> strength_tpl,
                                     tuple<int, int> defence_tpl, tuple<int, int> speed_tpl,
                                     tuple<int, int> luck_tpl,
                                     vector<CreatureSkill> attacking_skills,
                                     vector<CreatureSkill> defending_skills) : name(move(name)),
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
                                                                                    attacking_skills(move(
                                                                                            attacking_skills)),
                                                                                    defending_skills(move(
                                                                                            defending_skills)) {
    }

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

    float MagicalCreature::GetLuck() {
        return luck.get_current_value();
    }

    void MagicalCreature::PrintInitStats() {
        cout << "\n\n" << name << " is " << current_state.get_value() << ":\n\n"
                  << "Health: " << to_string(health.get_current_value()) << endl
                  << "Strength: " << to_string(strength.get_current_value()) << endl
                  << "Defence: " << to_string(defence.get_current_value()) << endl
                  << "Speed: " << to_string(speed.get_current_value()) << endl
                  << "Luck: " << to_string(luck.get_current_value()) << "%" << endl;

        PrintSkills();
    }

    void MagicalCreature::PrintSkills() {
        auto loop_skills_and_print = [](const string& skill_type, const vector<CreatureSkill>& skills) -> void {
            cout << "\n" << skill_type << " Skills: " << endl;
            for (auto skill: skills) {
                cout << skill.get_skill_name_with_desc() << endl;
            }
        };

        if (!attacking_skills.empty()) {
            loop_skills_and_print(ATTACKING, attacking_skills);
        } else {
            cout << "\n" << "Wild Beast doesn't have any special attacking skill." << endl;
        }

        if (!defending_skills.empty()) {
            loop_skills_and_print(DEFENDING, defending_skills);
        } else {
            cout << "\n" << "Wild Beast doesn't have any special defending skill." << endl;
        }
    }

    void MagicalCreature::SetCurrentState(MagicalCreatureState state) {
        current_state = std::move(state);
    }

    void MagicalCreature::UpdateDefenderHealthAndState(float damage_endured) {
        float health_left = health.get_current_value() - damage_endured;

        if (health_left <= 0) {
            health.SetHealth(0);
            this->SetCurrentState(Dead());
        } else {
            health.SetHealth(health_left);
        }
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
        return current_state;
    }

    const std::vector<CreatureSkill> &MagicalCreature::GetAttackingSkills() const {
        return attacking_skills;
    }

    const std::vector<CreatureSkill> &MagicalCreature::GetDefendingSkills() const {
        return defending_skills;
    }

    void MagicalCreature::SetCurrentLuckthreshold(double luck_threshold) {
        current_luck_threshold = luck_threshold;
    }

    bool MagicalCreature::IsLuckyEnoughToDodgeAttack() {
        return this->current_luck_threshold <= this->GetLuck();
    }

    bool MagicalCreature::IsLuckyEnoughToTriggerSkill(double skill_activation_percentage) {
            return this->current_luck_threshold <= skill_activation_percentage;
    }

    double MagicalCreature::GetCurrentLuckthreshold() const {
        return this->current_luck_threshold;
    }


}