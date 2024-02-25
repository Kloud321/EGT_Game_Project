#include <iostream>
#include "Game.h"


const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {

    Game game;

    if (!game.Init("Brick Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0)) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }

    game.RunGameLoop();

    game.Clean();

    // Clean up allocated memory

    return 0;
}
