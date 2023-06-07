#include "../../includes/game/PlayState.h"
#include "../../includes/creatures/FightManager.h"
#include "../../includes/creatures/skills/RapidStrike.h"
#include "../../includes/creatures/skills/MagicalShield.h"

using namespace std;
using namespace MagicalForestFights::Creatures;

namespace MagicalForestFights::Game {
    PlayState PlayState::playState;

    void PlayState::Init() {}

    void PlayState::Cleanup() {
        current_turn = 1;
    }

    void PlayState::Pause() {}

    void PlayState::Resume() {}

    void PlayState::HandleEvents(GameEngine *game) {
        MagicalCreature hero = MagicalCreature{
                "Natelus",
                tuple<int, int>(70, 100),
                tuple<int, int>(70, 80),
                tuple<int, int>(45, 55),
                tuple<int, int>(40, 50),
                tuple<int, int>(10, 30),
                std::vector<CreatureSkill>{RapidStrike()},
                std::vector<CreatureSkill>{MagicalShield()}
        };

        MagicalCreature wildbeast = {
                "Wild Beast",
                tuple<int, int>(60, 90),
                tuple<int, int>(60, 90),
                tuple<int, int>(40, 60),
                tuple<int, int>(40, 60),
                tuple<int, int>(24, 40),
                std::vector<CreatureSkill>{},
                std::vector<CreatureSkill>{}
        };

        FightManager fm = FightManager(hero, wildbeast);

        while (current_turn <= max_turns) {
//            usleep(2000000);
            fm.ProcessFight(current_turn, max_turns);

            if (fm.AnyMagicalCreatureDied()) {
                game->Quit();
                break;
            } else
                fm.swapAttDef();

            current_turn++;
        }

        if (current_turn > 20) {
            fm.SetFleeingState(max_turns);
            game->Quit();
        }
    }
}
