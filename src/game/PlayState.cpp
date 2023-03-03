#include <iostream>
#include <unistd.h>
#include "../../includes/game/PlayState.h"
#include "../../includes/creatures/FightManager.h"

using namespace std;
using namespace MagicalForestFights::Creatures;

namespace MagicalForestFights::Game {
    PlayState PlayState::playState;

    void PlayState::Init() {
        natelus = new MagicalCreature("Natelus", std::tuple<int, int>(70, 100),
                                      std::tuple<int, int>(70, 80),
                                      std::tuple<int, int>(45, 55), std::tuple<int, int>(40, 50),
                                      std::tuple<int, int>(10, 30),
                                      std::vector<CreatureSkill>{CreatureSkill(
                                              "Rapid Strike",
                                              Attack(),
                                              "Strike twice while it's his turn to attack, there's a 10% chance he'll use this skill every time he attacks",
                                              10, 2.0f)},
                                      std::vector<CreatureSkill>{CreatureSkill(
                                              "Magical Shield",
                                              Defense(),
                                              "Takes only half of the usual damage when an enemy attacks, there's a 20% chance he'll use this skill every time he defends.",
                                              20, 0.5f)});

        wild_beast = new MagicalCreature("Wild Beast", std::tuple<int, int>(60, 90),
                                         std::tuple<int, int>(60, 90),
                                         std::tuple<int, int>(40, 60), std::tuple<int, int>(40, 60),
                                         std::tuple<int, int>(25, 40),
                                         std::vector<CreatureSkill>{},
                                         std::vector<CreatureSkill>{});

        natelus->PrintInitStats();
        wild_beast->PrintInitStats();
    }

    void PlayState::Cleanup() {
        current_turn = 1;
    }

    void PlayState::Pause() {}

    void PlayState::Resume() {}

    void PlayState::HandleEvents(GameEngine *game) {
        FightManager fm = FightManager(natelus, wild_beast);
        cout << "\n\n" << fm.attacker->GetName() << " attacks first!" << endl;
        while (current_turn <= max_turns) {
//            usleep(2000000);
            auto [damage,
                  attacker_used_skill_output,
                  defender_dodged_output,
                  defender_used_skill_output] = fm.ProcessFight();

            fm.PrintFightInfo(damage, attacker_used_skill_output,
                              defender_dodged_output, defender_used_skill_output,
                              current_turn, max_turns);

            if (natelus->GetCurrentState() == Dead() || wild_beast->GetCurrentState() == Dead()) {
                game->Quit();
                break;
            } else {
                auto *temp = fm.attacker;
                fm.attacker = fm.defender;
                fm.attacker->SetCurrentState(Attacking());
                fm.defender = temp;
                fm.defender->SetCurrentState(Defending());
            }

            current_turn++;
        }

        if (current_turn > 20) {
            fm.attacker->SetCurrentState(Fleeing());
            fm.defender->SetCurrentState(Fleeing());

            std::cout << max_turns << "have passed and both Natellus and the Wild Beast are sill alive.\nThey were both too severely wounded to continue the fight and decided to flee!";
            std::cout << "GAME OVER" << std::endl;
            game->Quit();
        }
    }

    void PlayState::Update(GameEngine *game) {

    }
}
