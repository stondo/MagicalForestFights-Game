#include <cstring>
#include <string>
#include <sstream>
#include <unordered_map>
#include "spdlog/spdlog.h"
#include "../../includes/creatures/FightManager.h"
#include "../../includes/utils/CharContainer.h"

using namespace std;

namespace MagicalForestFights::Creatures {
    double FightManager::generateLuckThreshold() {
        random_device rd;
        static mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }

    template<typename... Strings>
    auto FightManager::concatenate(Strings... strings) {
        const auto totalSize = (0 + ... + strlen(strings));
        std::string result;
        result.reserve(totalSize);

        (result.append(strings), ...);
        return result;
    }

    void FightManager::ProcessFight(int current_turn, int max_turns) {
        _damage = 0.0;
        string _attackerUsedSkillOutput;
        string _defenderDodgedOutput;
        string _defenderUsedSkillOutput;

        attacker->SetCurrentLuckthreshold(generateLuckThreshold());
        defender->SetCurrentLuckthreshold(generateLuckThreshold());

        if (current_turn == 1) {
            attacker->PrintInitStats();
            defender->PrintInitStats();
        }

        spdlog::info("\n");
        spdlog::info("TURN # {}/{}", current_turn, max_turns);
        spdlog::info("{} attacks {}", attacker->GetName(), defender->GetName());

        spdlog::info( "{} luck threshold: {} - creature luck: {}", defender->GetName(),
                     defender->GetCurrentLuckthreshold(), defender->GetLuck());

        spdlog::info( "{} is {}", defender->GetName(),
                     hasLuck(defender->GetCurrentLuckthreshold(), defender->GetLuck()) ? "lucky" : "not lucky");

        auto def_dodged_f = [](const string &name, const double &curr_luck_threshold, const double &luck) -> string {
            std::stringstream ss;
            ss << name << " got lucky and dodged the attack! " <<
               "The luck threshold (" << curr_luck_threshold <<
               ") is less than or equal to the creature luck: " << luck;
            return ss.str();
        };

        auto used_skill_f = [](const string &name, const string &skill_name, const double &curr_luck_threshold,
                               const double &skill_activation_percentage) -> string {
            std::stringstream ss;
            ss << name << " has used " << skill_name << " skill! " <<
               "The luck threshold (" << curr_luck_threshold <<
               ") is less than or equal to the skill activation percentage: " << skill_activation_percentage;
            return ss.str();
        };

        if (!attacker->GetAttackingSkills().empty())
            for (const auto &skill: attacker->GetAttackingSkills()) {
                if (attacker->IsLuckyEnoughToTriggerSkill(skill.get_skill_activation_percentage()))
                    if (defender->IsLuckyEnoughToDodgeAttack()) {
                        _defenderDodgedOutput = def_dodged_f(defender->GetName(),
                                                             defender->GetCurrentLuckthreshold(),
                                                             defender->GetLuck());

                        _damage = 0.0;
                    } else {
                        _attackerUsedSkillOutput = used_skill_f(attacker->GetName(), skill.get_skill_name(),
                                                                attacker->GetCurrentLuckthreshold(),
                                                                skill.get_skill_activation_percentage());
                        _damage = skill.UseFn(skill.SKILL_FACTOR_FN)(calculateDamage());
                    }
                else if (defender->IsLuckyEnoughToDodgeAttack()) {
                    _defenderDodgedOutput = def_dodged_f(defender->GetName(),
                                                         defender->GetCurrentLuckthreshold(),
                                                         defender->GetLuck());
                    _damage = 0.0;
                } else
                    _damage = calculateDamage();
            }
        else if (defender->IsLuckyEnoughToDodgeAttack()) {
            _defenderDodgedOutput = def_dodged_f(defender->GetName(),
                                                 defender->GetCurrentLuckthreshold(),
                                                 defender->GetLuck());

            _damage = 0.0;
        }
        else if (!defender->GetDefendingSkills().empty())
            for (const auto &skill: defender->GetDefendingSkills()) {
                if (defender->IsLuckyEnoughToTriggerSkill(skill.get_skill_activation_percentage())) {
                    _defenderUsedSkillOutput = used_skill_f(defender->GetName(), skill.get_skill_name(),
                                                            defender->GetCurrentLuckthreshold(),
                                                            skill.get_skill_activation_percentage());
                    _damage = skill.UseFn(skill.SKILL_FACTOR_FN)(calculateDamage());
                } else
                    _damage = calculateDamage();
            }
        else
            _damage = calculateDamage();

        if (_damage > 0) defender->UpdateDefenderHealthAndState(_damage);
        else _damage = 0;

        if (!_attackerUsedSkillOutput.empty())
            spdlog::warn(_attackerUsedSkillOutput);

        if (!_defenderUsedSkillOutput.empty())
            spdlog::warn(_defenderUsedSkillOutput);

        if (!_defenderDodgedOutput.empty())
            spdlog::warn(_defenderDodgedOutput);

        auto f = [](float d) -> string { return d != 0.0 ? to_string(d) : "no"; };
        spdlog::info("{} has dealt {} damage to {}", attacker->GetName(), f(_damage), defender->GetName());
        spdlog::info("{}'s health left: {}", defender->GetName(), defender->GetHealth());

        if (defender->GetHealth() == 0) {
            spdlog::critical("{} has killed {}", attacker->GetName(), defender->GetName());
            spdlog::critical("GAME OVER");
        }
    }

    void FightManager::setAttackerAndDefender() {
        if ((defender->GetSpeed() == attacker->GetSpeed() && defender->GetLuck() >= attacker->GetLuck()) ||
            defender->GetSpeed() > attacker->GetSpeed())
                swap(attacker, defender);

        attacker->SetCurrentState(Attacking());
        defender->SetCurrentState(Defending());

        spdlog::info("\n\n {}attacks first!", attacker->GetName());
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
        swap(attacker, defender);
        attacker->SetCurrentState(Attacking());
        defender->SetCurrentState(Defending());
    }

    void FightManager::SetFleeingState(int max_turns) {
        attacker->SetCurrentState(Fleeing());
        defender->SetCurrentState(Fleeing());
        spdlog::info("{} have passed and both {} and {} are sill alive.\nThey were both too severely wounded to continue the fight and decided to flee!",
                     max_turns, attacker->GetName(), defender->GetName());
        spdlog::info("GAME OVER");
    }

    bool FightManager::AnyMagicalCreatureDied() {
        return attacker->GetCurrentState() == Dead() || defender->GetCurrentState() == Dead();
    }

    FightManager::FightManager() {
        readInitDataAndCreateMagicalCreatures();
        setAttackerAndDefender();
    }

    FightManager::FightManager(const MagicalCreature &hero, const MagicalCreature &beast) {
        attacker = make_unique<MagicalCreature>(hero);
        defender = make_unique<MagicalCreature>(beast);
        setAttackerAndDefender();
    }

    void FightManager::readInitDataAndCreateMagicalCreatures() {
        spdlog::info("Reading config file");

        auto extractCreatureStats = [](const ryml::ConstNodeRef &creature) -> unique_ptr<MagicalCreature> {
            auto skillFactoryLambda = [](const string &skillName, SkillType skillType, const string &skillDesc, double ap, float sf) -> CreatureSkill {
                return {skillName, skillType, skillDesc, ap, sf};
            };

            unordered_map<string, decltype(skillFactoryLambda)> availableSkillsMap = {
                    {"Rapid Strike", skillFactoryLambda},
                    {"Magical Shield", skillFactoryLambda}
            };

            unordered_map<string, SkillType> skillTypeMap = {
                    {"attack", SkillType::ATTACK},
                    {"defense", SkillType::DEFENSE}
            };

            string heroName;
            unordered_map<int, unordered_map<int, int>> creaturePropsMap = {
                    {CreatureProperty::HEALTH,   {{MIN, 0}, {MAX, 0}}},
                    {CreatureProperty::STRENGTH, {{MIN, 0}, {MAX, 0}}},
                    {CreatureProperty::DEFENSE,  {{MIN, 0}, {MAX, 0}}},
                    {CreatureProperty::SPEED,    {{MIN, 0}, {MAX, 0}}},
                    {CreatureProperty::LUCK,     {{MIN, 0}, {MAX, 0}}}
            };
            vector<unordered_map<int, string>> creatureSkills;

            creature[NAME] >> heroName;

            creature[HEALTH][MIN] >> creaturePropsMap[HEALTH][MIN];
            creature[HEALTH][MAX] >> creaturePropsMap[HEALTH][MAX];

            creature[STRENGTH][MIN] >> creaturePropsMap[STRENGTH][MIN];
            creature[STRENGTH][MAX] >> creaturePropsMap[STRENGTH][MAX];

            creature[DEFENSE][MIN] >> creaturePropsMap[DEFENSE][MIN];
            creature[DEFENSE][MAX] >> creaturePropsMap[DEFENSE][MAX];

            creature[SPEED][MIN] >> creaturePropsMap[SPEED][MIN];
            creature[SPEED][MAX] >> creaturePropsMap[SPEED][MAX];

            creature[LUCK][MIN] >> creaturePropsMap[LUCK][MIN];
            creature[LUCK][MAX] >> creaturePropsMap[LUCK][MAX];

            unordered_map<int, string> creatureSkillsMap = {
                    {CreatureSkillProperty::SKILL_NAME, ""},
                    {CreatureSkillProperty::TYPE, ""},
                    {CreatureSkillProperty::DESC, ""},
                    {CreatureSkillProperty::ACTIVATION_PERCENTAGE, ""},
                    {CreatureSkillProperty::SKILL_FACTOR, ""}
            };

            for (size_t k = 0; k < creature[SKILLS].num_children(); k++) {
                for (auto skillProp: CreatureSkillProperties)
                    creature[SKILLS][k][skillProp] >> creatureSkillsMap[skillProp];

                creatureSkills.push_back(creatureSkillsMap);
            }

            vector<CreatureSkill> attackingSkills;
            vector<CreatureSkill> defendingSkills;

            for ( auto &cskill : creatureSkills ) {
                auto skill = availableSkillsMap[cskill[CreatureSkillProperty::SKILL_NAME]](
                        cskill[CreatureSkillProperty::SKILL_NAME],
                        skillTypeMap[cskill[CreatureSkillProperty::TYPE]],
                        cskill[CreatureSkillProperty::DESC],
                        std::stod(cskill[CreatureSkillProperty::ACTIVATION_PERCENTAGE]),
                        std::stof(cskill[CreatureSkillProperty::SKILL_FACTOR])
                        );

                if (skill.skill_type == SkillType::ATTACK) attackingSkills.push_back(skill);
                else if (skill.skill_type == SkillType::DEFENSE) defendingSkills.push_back(skill);
            }

            unique_ptr<MagicalCreature> newCreature = make_unique<MagicalCreature>(
                    heroName,
                    tuple<int, int>(creaturePropsMap[HEALTH][MIN], creaturePropsMap[HEALTH][MAX]),
                    tuple<int, int>(creaturePropsMap[STRENGTH][MIN], creaturePropsMap[STRENGTH][MAX]),
                    tuple<int, int>(creaturePropsMap[DEFENSE][MIN], creaturePropsMap[DEFENSE][MAX]),
                    tuple<int, int>(creaturePropsMap[SPEED][MIN], creaturePropsMap[SPEED][MAX]),
                    tuple<int, int>(creaturePropsMap[LUCK][MIN], creaturePropsMap[LUCK][MAX]),
                    attackingSkills,
                    defendingSkills
            );

            return newCreature;
        };

        const char filename[] = "config.yaml";
        auto contents = file_get_contents<std::string>(filename);

        ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(contents)); // immutable (csubstr) overload

        ryml::ConstNodeRef heroes = tree["heroes"];
        ryml::ConstNodeRef monsters = tree["monsters"];

        vector<unique_ptr<MagicalCreature>> mcs;

        for (size_t i = 0; i < heroes.num_children(); i++)
            mcs.push_back(extractCreatureStats(heroes[i]));

        for (size_t i = 0; i < monsters.num_children(); i++)
            mcs.push_back(extractCreatureStats(monsters[i]));

        attacker = std::move(mcs[0]);
        defender = std::move(mcs[1]);
    }

}