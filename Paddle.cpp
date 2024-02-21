#include "Paddle.h"

Paddle::Paddle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

void Paddle::MoveLeft() {
    x -= 10;
}

void Paddle::MoveRight() {
    x += 10;
}

void Paddle::Render(SDL_Renderer* renderer) {
    SDL_Rect paddleRect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddleRect);
    //std::cout << "Rendering paddle\n";
}

SDL_Rect Paddle::GetRect() {
    return { x, y, width, height };
}


void Paddle::setWidth(int width) {

    this->width = width;
}

void Paddle::setHeight(int height) {

    this->height = width;
}

int Paddle::getWidth() {
    return width;
}

int Paddle::getHeight() {
    return height;
}

void Paddle::setX(int x) {

    this->x = x;
}

void Paddle::setY(int y) {

    this->y = y;
}

int Paddle::getX() {
    return x;
}

int Paddle::getY() {
    return y;
}

