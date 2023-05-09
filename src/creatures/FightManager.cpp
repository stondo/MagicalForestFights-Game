#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "rapidcsv.h"
#include "spdlog/spdlog.h"
#include "../../includes/creatures/FightManager.h"
#include "../../includes/utils/CharContainer.h"

#include <ryml.hpp>
#include <ryml_std.hpp> // optional header, provided for std:: interop

using namespace std;

namespace MagicalForestFights::Creatures {
    double FightManager::generateLuckThreshold() {
        random_device rd;
        static mt19937 gen(rd());
        static uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }

    template <typename... Strings>
    auto FightManager::concatenate(Strings... strings)
    {
        const auto totalSize = (0 + ... + strlen(strings));
        std::string result;
        result.reserve(totalSize);

        (result.append(strings), ...);
        return result;
    }

    void FightManager::ProcessFight() {
        attacker->SetCurrentLuckthreshold(generateLuckThreshold());
        defender->SetCurrentLuckthreshold(generateLuckThreshold());

        auto def_dodged_f = [](const string& name, const double& curr_luck_threshold, const double& luck) -> string {
            std::stringstream ss;
            ss << name << " got lucky and dodged the attack! " <<
            "The luck threshold (" << curr_luck_threshold <<
            ") is less than or equal to the creature luck: " << luck;
            return ss.str();
        };

        auto used_skill_f = [](const string& name, const string& skill_name, const double& curr_luck_threshold, const double& skill_activation_percentage) -> string {
            std::stringstream ss;
            ss << name << " has used " << skill_name << " skill! " <<
               "The luck threshold (" << curr_luck_threshold <<
               ") is less than or equal to the skill activation percentage: " << skill_activation_percentage;
            return ss.str();
        };

        if (!attacker->GetAttackingSkills().empty())
            for (const auto& skill : attacker->GetAttackingSkills()) {
                if (attacker->IsLuckyEnoughToTriggerSkill(skill.get_skill_activation_percentage()))
                    if (defender->IsLuckyEnoughToDodgeAttack())
                        _defenderDodgedOutput = def_dodged_f(defender->GetName(),
                                                             defender->GetCurrentLuckthreshold(),
                                                             defender->GetLuck());
                    else {
                        _attackerUsedSkillOutput = used_skill_f(attacker->GetName(), skill.get_skill_name(), attacker->GetCurrentLuckthreshold(), skill.get_skill_activation_percentage());
                        _damage = skill.UseFn(SKILL_FACTOR_FN)(calculateDamage());
                    }
                else
                    if (defender->IsLuckyEnoughToDodgeAttack())
                        _defenderDodgedOutput = def_dodged_f(defender->GetName(),
                                                             defender->GetCurrentLuckthreshold(),
                                                             defender->GetLuck());
                    else
                        _damage = calculateDamage();
            }
        else
            if (defender->IsLuckyEnoughToDodgeAttack())
                _defenderDodgedOutput = def_dodged_f(defender->GetName(),
                                                     defender->GetCurrentLuckthreshold(),
                                                     defender->GetLuck());
            else
                if (!defender->GetDefendingSkills().empty())
                    for (const auto& skill : defender->GetDefendingSkills()) {
                        if (defender->IsLuckyEnoughToTriggerSkill(skill.get_skill_activation_percentage())) {
                            _defenderUsedSkillOutput = used_skill_f(defender->GetName(), skill.get_skill_name(), defender->GetCurrentLuckthreshold(), skill.get_skill_activation_percentage());
                            _damage = skill.UseFn(SKILL_FACTOR_FN)(calculateDamage());
                        } else
                            _damage = calculateDamage();
                    }
                else
                    _damage = calculateDamage();

        if (_damage > 0) defender->UpdateDefenderHealthAndState(_damage);
        else _damage = 0;
    }

    void FightManager::setAttackerAndDefender(const std::shared_ptr<MagicalCreature>& hero, const std::shared_ptr<MagicalCreature>& beast) {
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

        spdlog::info("\n\n {} attacks first!", attacker->GetName());
    }

    void FightManager::PrintFightInfo(int current_turn, int max_turns) {
        spdlog::info("TURN # {}/{}", current_turn, max_turns);
        spdlog::info("{} attacks {}", attacker->GetName(), defender->GetName());
        spdlog::info(defender->GetName() + " luck threshold: {} - creature luck: {}", defender->GetCurrentLuckthreshold(), defender->GetLuck());
        spdlog::info(defender->GetName() + " is {}", hasLuck(defender->GetCurrentLuckthreshold(), defender->GetLuck()) ? "lucky" : "not lucky");

        if (!_attackerUsedSkillOutput.empty())
            spdlog::warn(_attackerUsedSkillOutput);


        if (!_defenderUsedSkillOutput.empty())
            spdlog::warn(_defenderUsedSkillOutput);

        if (!_defenderDodgedOutput.empty())
            spdlog::warn(_defenderDodgedOutput);

        auto f = [](float d) -> string { return d != 0.0 ? to_string(d) : "no"; };
        spdlog::info("{} has dealt {} _damage to {}", attacker->GetName(), f(_damage), defender->GetName());
        spdlog::info("{}'s health left: {}", defender->GetName(), defender->GetHealth());

        if (defender->GetHealth() == 0) {
            spdlog::critical("{} has killed {}", attacker->GetName(), defender->GetName());
            spdlog::critical("GAME OVER");
        }
    }

    bool FightManager::hasLuck(double luck_threshold, float creature_luck) {
        return luck_threshold <= creature_luck;
    }

    float FightManager::calculateDamage() const {
        return float(attacker->GetStrength() - defender->GetDefence());
    }

    float FightManager::GetDamageDone() const {
        return _damage;
    }

    void FightManager::swapAttDef() {
        std::swap(attacker, defender);
        attacker->SetCurrentState(Attacking());
        defender->SetCurrentState(Defending());
    }

    void FightManager::SetFleeingState() {
        attacker->SetCurrentState(Fleeing());
        defender->SetCurrentState(Fleeing());
    }

    bool FightManager::AnyMagicalCreatureDied() {
        return attacker->GetCurrentState() == Dead() || defender->GetCurrentState() == Dead();
    }

    FightManager::FightManager() {
        readInitDataAndCreateMagicalCreatures();
        setAttackerAndDefender(attacker, defender);
    }

    FightManager::FightManager(std::shared_ptr<MagicalCreature> hero, std::shared_ptr<MagicalCreature> beast) {
        setAttackerAndDefender(hero, beast);
    }

    void FightManager::readInitDataAndCreateMagicalCreatures() {

//        auto getSkills =  [](string s) -> vector<string> {
//            vector<string> res;
//            int pos = 0;
//            while(pos < s.size()){
//                pos = s.find("|");
//                res.push_back(s.substr(0,pos));
//                s.erase(0,pos+1); // 1 is the length of the delimiter, "|"
//            }
//            return res;
//        };
//
//        rapidcsv::Document doc("creatures.csv", rapidcsv::LabelParams(0, 0));
//
//        vector<CreatureSkill> attackingSkills;
//        vector<CreatureSkill> defendingSkills;
//
//        vector<MagicalCreature> creatures;
//        for (auto &rowName : doc.GetRowNames()) {
//            spdlog::info("Row Name: {}", rowName);
//
//            auto healthMin = doc.GetCell<int>("HealthMin", rowName);
//            auto healthMax = doc.GetCell<int>("HealthMax", rowName);
//
//            auto strengthMin = doc.GetCell<int>("StrengthMin", rowName);
//            auto strengthMax = doc.GetCell<int>("StrengthMax", rowName);
//
//            auto defenceMin = doc.GetCell<int>("DefenceMin", rowName);
//            auto defenceMax = doc.GetCell<int>("DefenceMax", rowName);
//
//            auto speedMin = doc.GetCell<int>("SpeedMin", rowName);
//            auto speedMax = doc.GetCell<int>("SpeedMax", rowName);
//
//            auto luckMin = doc.GetCell<int>("LuckMin", rowName);
//            auto luckMax = doc.GetCell<int>("LuckMax", rowName);
//
//            auto attackingSkillsStr = doc.GetCell<std::string>("AttackingSkills", rowName);
//            auto defendingSkillsStr = doc.GetCell<std::string>("DefendingSkills", rowName);
//
//            spdlog::error("AttackingSkills: {}", getSkills(attackingSkillsStr)[0]);
//            spdlog::error("DefendingSkills: {}", getSkills(defendingSkillsStr)[0]);
//
//            auto attackingSkills = getSkills(attackingSkillsStr);
//
//            auto creature = new MagicalCreature(rowName, std::tuple<int, int>(healthMin, healthMax),
//                              std::tuple<int, int>(strengthMin, strengthMax),
//                              std::tuple<int, int>(defenceMin, defenceMax), std::tuple<int, int>(speedMin, speedMax),
//                              std::tuple<int, int>(luckMin, luckMax),
//                              std::vector<CreatureSkill>{CreatureSkill(
//                                      "Rapid Strike",
//                                      Attack(),
//                                      "Strike twice while it's his turn to attack, there's a 10% chance he'll use this skill every time he attacks",
//                                      10, 2.0f)},
//                              std::vector<CreatureSkill>{CreatureSkill(
//                                      "Magical Shield",
//                                      Defense(),
//                                      "Takes only half of the usual _damage when an enemy attacks, there's a 20% chance he'll use this skill every time he defends.",
//                                      20, 0.5f)});
//            creatures.push_back(*creature);
//        }
//
//        attacker =  std::make_unique<MagicalCreature>(creatures[0]);
//        defender = std::make_unique<MagicalCreature>(creatures[1]);

        std::cout << "Reading config file" << std::endl;
        const char filename[] = "config.yaml";
        auto contents = file_get_contents<std::string>(filename);

//        ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(contents)); // immutable (csubstr) overload
//        std:: cout << tree["heroes"].key() << std::endl;

        // ryml::ConstNodeRef root = tree.crootref();

//        ryml::ConstNodeRef heroes = tree["heroes"];
//        ryml::ConstNodeRef monsters = tree["monsters"];

//        for (size_t i = 0; i < heroes.num_children(); i++) {
//            std::cout << "i is: " << i << std::endl;

//            for (auto prop : CreatureProperties) {
//                std::cout << std::to_string(prop) << std::endl;
//            }

//            std::cout << heroes[i]["skills"].first_child()[3] << std::endl;
//            std::cout << heroes[i]["skills"].first_child()[3] << std::endl;
//        }

//        std::vector<ryml::csubstr> keys, vals; // to store all the root-level keys, vals
//        for(ryml::ConstNodeRef n : root.children())
//        {
//            keys.emplace_back(n.key());
//            std:: cout << "KEY: " << n.key() << std::endl;
//
//            vals.emplace_back(n.has_val() ? n.val() : ryml::csubstr{});
//            std:: cout << "VAL: " << (n.has_val() ? n.val() : ryml::csubstr{}) << std::endl;
//        }
    }
}