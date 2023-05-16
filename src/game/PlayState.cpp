#include "../../includes/game/PlayState.h"
#include "../../includes/creatures/FightManager.h"

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
        FightManager fm = FightManager();

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
