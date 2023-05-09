#ifndef MAGICALFOESTFIGHTS_PLAYSTATE_H
#define MAGICALFOESTFIGHTS_PLAYSTATE_H


#include <tuple>
#include "GameState.h"

using namespace MagicalForestFights::Creatures;

namespace MagicalForestFights::Game {
    class PlayState : public GameState {
    public:
        void Init();

        void Cleanup();

        void Pause();

        void Resume();

        void HandleEvents(GameEngine *game);

        void Update(GameEngine *game);

        static PlayState *Instance() {
            return &playState;
        }

    protected:
        PlayState() {}

    private:
        static PlayState playState;

//        MagicalCreature *natelus;
//        MagicalCreature *wild_beast;
    };
}

#endif //MAGICALFOESTFIGHTS_PLAYSTATE_H
