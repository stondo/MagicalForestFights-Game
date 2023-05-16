#include <string>
#include <iostream>
#include "../../includes/game/GameEngine.h"

namespace MagicalForestFights::Game {
    void GameEngine::Init(const std::string &title, int max_turns) {
        std::cout << title << std::endl;
        running = true;
        turns = max_turns;
    }

    void GameEngine::Cleanup() {
        // cleanup the all states
        while (!states.empty()) {
            states.back()->Cleanup();
            states.pop_back();
        }
    }

    void GameEngine::ChangeState(GameState *state) {
        // cleanup the current state
        if (!states.empty()) {
            states.back()->Cleanup();
            states.pop_back();
        }

        // store and init the new state
        state->max_turns = this->turns;
        states.push_back(state);
        states.back()->Init();
    }

    [[maybe_unused]] void GameEngine::PushState(GameState *state) {
        // pause current state
        if (!states.empty()) {
            states.back()->Pause();
        }

        // store and init the new state
        states.push_back(state);
        states.back()->Init();
    }

    [[maybe_unused]] void GameEngine::PopState() {
        // cleanup the current state
        if (!states.empty()) {
            states.back()->Cleanup();
            states.pop_back();
        }

        // resume previous state
        if (!states.empty()) {
            states.back()->Resume();
        }
    }


    void GameEngine::HandleEvents() {
        // let the state handle events
        states.back()->HandleEvents(this);
    }

//    void GameEngine::Update() {
//        // let the state update the game
//        states.back()->Update(this);
//    }
}