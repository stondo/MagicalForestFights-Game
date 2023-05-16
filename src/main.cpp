#include "../includes/game/GameEngine.h"
#include "../includes/game/IntroState.h"

using namespace MagicalForestFights::Game;

int main() {
    GameEngine game;
    game.Init( "Magical Forest Engine v1.0", 20);
    game.ChangeState( IntroState::Instance() );

    while ( game.Running() ) {
        game.HandleEvents();
    }

    game.Cleanup();
    return 0;
}
