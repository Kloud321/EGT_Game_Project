#include "Bricks.h"

Brick::Brick(int x, int y, int width, int height, bool isGray) : x(x), y(y), width(width), height(height), isGray(isGray), durability(isGray ? 2 : 1) {}

void Brick::Render(SDL_Renderer* renderer) {
    SDL_Rect brickRect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, isGray ? 128 : 255, isGray ? 128 : 255, 0, 255);
    SDL_RenderFillRect(renderer, &brickRect);
    std::cout << "Rendering bricks\n";
}

bool Brick::IsBroken() const {
    return durability <= 0;
}

void Brick::Hit() {
    durability -= 1;
}

SDL_Rect Brick::GetRect() const {
    return { x, y, width, height };
}
