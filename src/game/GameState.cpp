//
// Created by Stefano Tondo on 24.02.23.
//
#include "../../includes/game/GameState.h"

namespace MagicalForestFights::Game {
    void GameState::ChangeState(GameEngine *game, GameState *state) {
        game->ChangeState(state);
    }
}
