#ifndef MAGICALFOESTFIGHTS_INTROSTATE_H
#define MAGICALFOESTFIGHTS_INTROSTATE_H

#include <string>
#include "GameState.h"

namespace MagicalForestFights::Game {
    class IntroState : public GameState {
    public:
        void Init();

        void Cleanup();

        void Pause();

        void Resume();

        void HandleEvents(GameEngine *game);

        void Update(GameEngine *game);

        static IntroState *Instance() {
            return &introState;
        }

    protected:
        IntroState() {}

    private:
        static IntroState introState;
        std::string intro_desc;
    };
}

#endif //MAGICALFOESTFIGHTS_INTROSTATE_H
