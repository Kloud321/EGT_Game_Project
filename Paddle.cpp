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

void Paddle::setX(int x) {

    this->x = x;
}

void Paddle::setY(int y) {

    this->y = y;
}

int Paddle::getWidth() const {
    return width;
}

int Paddle::getHeight() const {
    return height;
}

int Paddle::getX() const{
    return x;
}

int Paddle::getY() const{
    return y;
}

