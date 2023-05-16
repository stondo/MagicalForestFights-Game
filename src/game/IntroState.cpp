#include <iostream>
#include <string>
#include "../../includes/game/IntroState.h"
#include "../../includes/game/PlayState.h"

namespace MagicalForestFights::Game {
    IntroState IntroState::introState;

    void IntroState::Init() {
        intro_desc = "Welcome to the story of the mighty hero Natelus! He is about to enter the perilous and magical ever-green forest full of wild beasts and treasures.\nWill he be able to make it out alive?\n\n";
        std::cout << intro_desc << std::endl;
    }

    void IntroState::Cleanup() {
    }

    void IntroState::Pause() {
    }

    void IntroState::Resume() {
    }

    void IntroState::HandleEvents(GameEngine *game) {
        std::string user_input;

        std::cout << "Type yes to find out: ";
        std::getline(std::cin, user_input);

        if (user_input == "yes" || user_input == "YES") {
            std::cout << "You typed yes, so let's find out!" << std::endl;
            game->ChangeState(PlayState::Instance());
        } else {
            std::cout << "You didn't type yes, so you'll never know!" << std::endl;
            game->Quit();
        }
    }
}
