#ifndef MAGICALFOESTFIGHTS_GAMESTATE_H
#define MAGICALFOESTFIGHTS_GAMESTATE_H


#include "GameEngine.h"

namespace MagicalForestFights::Game {
    class GameEngine;

    class GameState {
    public:
        virtual void Init() = 0;

        virtual void Cleanup() = 0;

        virtual void Pause() = 0;

        virtual void Resume() = 0;

        virtual void HandleEvents(GameEngine *game) = 0;

        virtual void Update(GameEngine *game) = 0;

        void ChangeState(GameEngine *game, GameState *state);

        int max_turns = 0;
        int current_turn = 1;

    protected:
        GameState() {}
    };
}

#endif //MAGICALFOESTFIGHTS_GAMESTATE_H
