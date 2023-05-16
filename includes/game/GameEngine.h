#ifndef MAGICALFOESTFIGHTS_GAMEENGINE_H
#define MAGICALFOESTFIGHTS_GAMEENGINE_H

#include <string>
#include <vector>
#include "GameState.h"
#include "../creatures/MagicalCreature.h"


namespace MagicalForestFights::Game {
    class GameState;

    class GameEngine {
    public:
        void Init(const std::string &title, int max_turns);

        void Cleanup();

        void ChangeState(GameState *state);

        [[maybe_unused]] void PushState(GameState *state);

        [[maybe_unused]] void PopState();

        void HandleEvents();

        void Update();

        bool Running() { return running; }

        void Quit() { running = false; }

    private:
        // the stack of states
        std::vector<GameState *> states;

        bool running = false;
        int turns = 0;
    };
}


#endif //MAGICALFOESTFIGHTS_GAMEENGINE_H
