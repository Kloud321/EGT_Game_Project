#include <iostream>
#include "Game.h"

Game* game = nullptr;
const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
    game = new Game();
    game->Init("Brick Breaker",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE);

    game->RunGameLoop();

    game->Clean();
    delete game; // Clean up allocated memory

    return 0;
}
