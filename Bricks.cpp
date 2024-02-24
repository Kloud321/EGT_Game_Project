#include "Bricks.h"

Brick::Brick(int x, int y, int width, int height, bool isGray) : x(x), y(y), width(width), height(height), isGray(isGray), durability(isGray ? 2 : 1) {}

void Brick::Render(SDL_Renderer* renderer) {
    SDL_Rect brickRect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, isGray ? 160 : 255, isGray ? 160 : 165, isGray ? 160 : 0, 255);
    SDL_RenderFillRect(renderer, &brickRect);

    if (isGray) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
        //Outline for bricks
        SDL_RenderDrawRect(renderer, &brickRect);
    }
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

int Brick::getX() const {
    return this->x;
}

int Brick::getY() const {
    return this->y;
}

int Brick::getWidth() const {
    return this->width;

}

int Brick::getHeight() const {
    return this->height;

}

int Brick::getDurability() const {
    return this->durability;
}

bool Brick::getIsGray() const {
    return this->isGray;
}

void Brick::setIsGray(bool newValue) {
    this->isGray = newValue;
}