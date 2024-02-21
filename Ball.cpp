#include "Ball.h"

Ball::Ball(int x, int y, int radius, int velocityX, int velocityY, int windowWidth, int windowHeight)
    : x(x), y(y), radius(radius), velocityX(velocityX), velocityY(velocityY), windowWidth(windowWidth), windowHeight(windowHeight) {}

void Ball::Update() {

    //ball position
    x += velocityX;
    y += velocityY;


    
 if (x - radius < 0 || x + radius > windowWidth) {
     velocityX = -velocityX; // Reverse horizontal velocity
 }
 if (y - radius < 0 || y + radius > windowHeight) {
    velocityY = -velocityY; // Reverse vertical velocity
    }

}

void Ball::Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = GetRect();
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    //std::cout << "Rendering ball\n";
}

SDL_Rect Ball::GetRect(){
    return { x - radius, y - radius, radius * 2, radius * 2 };
}

void Ball::ChangeDirectionX() {
    velocityX = -velocityX;
}

void Ball::ChangeDirectionY() {
    velocityY = -velocityY;
}


void Ball::setX(int x) {

    x = this->x;
}

void Ball::setY(int y) {

    y = this->y;
}

int Ball::getX() {
    return x;
}

int Ball::getY() {
    return y;
}

int Ball::getRadius() {
    return radius;
}

void Ball::setRadius(int radius) {
    this->radius = radius;
}

int Ball::getVelocityX() {
    return this->velocityX;
}

void Ball::setVelocityX(int velocityX) {
    this->velocityX = velocityX;
}

int Ball::getVelocityY() {
    return this->velocityY;
}

void Ball::setVelocityY(int velocityY) {
    this->velocityY = velocityY;
}