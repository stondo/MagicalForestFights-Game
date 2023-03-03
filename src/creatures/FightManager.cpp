#include <iostream>
#include "../../includes/creatures/FightManager.h"

using namespace std;

namespace MagicalForestFights::Creatures {
    double FightManager::generate_luck_threshold() {
        // we make the generator and distribution 'static' to keep their value
        // across calls to the function.
        random_device rd;
        static mt19937 gen(rd());
        static uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }

    tuple<float, string, string, string> FightManager::ProcessFight() {
        float damage = 0.0;
        string attacker_used_skill_output;
        string defender_dodged_output;
        string defender_used_skill_output;

        attacker->SetCurrentLuckthreshold(generate_luck_threshold());
        defender->SetCurrentLuckthreshold(generate_luck_threshold());

        auto def_dodged_f = [](const string& name, const string& curr_luck_threshold, const string& luck) -> string {
            return name + " got lucky and dodged the attack!\n" +
            "because luck threshold (" + curr_luck_threshold +
            ") is less than or equal to creature luck: " + luck;
        };

        if (!attacker->GetAttackingSkills().empty()) {
            for (const auto& skill : attacker->GetAttackingSkills()) {
                if (attacker->IsLuckyEnoughToTriggerSkill(skill.get_skill_activation_percentage())) {
                    if (defender->IsLuckyEnoughToDodgeAttack()) {
                        defender_dodged_output = def_dodged_f(defender->GetName(),
                                                              to_string(defender->GetCurrentLuckthreshold()),
                                                              to_string(defender->GetLuck()));
                    } else {
                        attacker_used_skill_output = attacker->GetName() + " has used " + skill.get_skill_name() +
                                " skill!\n" + "because luck threshold (" + to_string(attacker->GetCurrentLuckthreshold()) +
                                ") is less than or equal to creature skill activation percentage: " +
                                to_string(skill.get_skill_activation_percentage());

                        auto applySkill = skill.UseFn(SKILL_FACTOR_FN);
                        damage = applySkill(float(attacker->GetStrength() - defender->GetDefence()));
                    }
                } else {
                    if (defender->IsLuckyEnoughToDodgeAttack()) {
                        defender_dodged_output = def_dodged_f(defender->GetName(),
                                                              to_string(defender->GetCurrentLuckthreshold()),
                                                              to_string(defender->GetLuck()));
                    } else {
                        damage = float(attacker->GetStrength() - defender->GetDefence());
                    }
                }
            }
        } else {
            if (defender->IsLuckyEnoughToDodgeAttack()) {
                defender_dodged_output = def_dodged_f(defender->GetName(),
                                                      to_string(defender->GetCurrentLuckthreshold()),
                                                      to_string(defender->GetLuck()));
            } else {
                if (!defender->GetDefendingSkills().empty()) {
                    for (auto skill : defender->GetDefendingSkills()) {
                        if (defender->IsLuckyEnoughToTriggerSkill(skill.get_skill_activation_percentage())) {
                            defender_used_skill_output = defender->GetName() + " has used " + skill.get_skill_name() +
                                    " skill!\n" + "because luck threshold (" + to_string(defender->GetCurrentLuckthreshold()) +
                                    ") is less than or equal to creature skill activation percentage: " +
                                    to_string(skill.get_skill_activation_percentage());

                            auto applySkill = skill.UseFn(SKILL_FACTOR_FN);
                            damage = applySkill(float(attacker->GetStrength() - defender->GetDefence()));
                        } else {
                            damage = float(attacker->GetStrength() - defender->GetDefence());
                        }
                    }
                } else {
                    damage = float(attacker->GetStrength() - defender->GetDefence());
                }

            }
        }

        if (damage > 0) {
            defender->UpdateDefenderHealthAndState(damage);
        } else {
            damage = 0;
        }

        return {damage, attacker_used_skill_output,
                defender_dodged_output,
                defender_used_skill_output};
    }

    void FightManager::set_attacker_and_defender(MagicalCreature *hero, MagicalCreature *beast) {
        if ((hero->GetSpeed() == beast->GetSpeed() && hero->GetLuck() >= beast->GetLuck()) ||
            hero->GetSpeed() > beast->GetSpeed()) {
            hero->SetCurrentState(Attacking());
            beast->SetCurrentState(Defending());
            this->attacker = hero;
            this->defender = beast;
        } else {
            beast->SetCurrentState(Attacking());
            hero->SetCurrentState(Defending());
            this->attacker = beast;
            this->defender = hero;
        }
    }

    void FightManager::PrintFightInfo(float damage, const string& attacker_used_skill_output, const string& defender_dodged_output,
                                      const string& defender_used_skill_output, int current_turn, int max_turns) {
        cout << "\n\nTURN #" << current_turn << "/" << max_turns << endl;
        cout << attacker->GetName() << " attacks " << defender->GetName() << endl;

        cout << defender->GetName() << " luck threshold: " << defender->GetCurrentLuckthreshold() << " - creature luck: " << defender->GetLuck()
             << endl;
        cout << defender->GetName() << " is " << (hasLuck(defender->GetCurrentLuckthreshold(), defender->GetLuck()) ? "lucky" : "not lucky")
             << " this turn." << endl;

        if (!attacker_used_skill_output.empty()) {
            cout << attacker_used_skill_output << endl;
        }

        if (!defender_used_skill_output.empty()) {
            cout << defender_used_skill_output << endl;
        }

        if (!defender_dodged_output.empty()) {
            cout << defender_used_skill_output << endl;
        }

        auto f = [](float d) -> string { return d != 0.0 ? to_string(d) : "no"; };
        cout << attacker->GetName() << " has dealt " << f(damage) << " damage to " << defender->GetName()
             << endl;
        cout << defender->GetName() << "'s health left: " << defender->GetHealth() << endl;

        if (defender->GetHealth() == 0) {
            cout << attacker->GetName() << " killed " << defender->GetName() << endl;
            cout << "GAME OVER" << endl;
        }
    }

    bool FightManager::hasLuck(double luck_threshold, float creature_luck) {
        return luck_threshold <= creature_luck;
    }

    FightManager::FightManager(MagicalCreature *hero, MagicalCreature *beast) {
        set_attacker_and_defender(hero, beast);
    }

}