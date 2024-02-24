#include "Ball.h"
#include "Game.h"

Ball::Ball(int x, int y, int radius, int velocityX, int velocityY, int windowWidth, int windowHeight)
    : x(x), y(y), radius(radius), velocityX(velocityX), velocityY(velocityY), windowWidth(windowWidth),
    windowHeight(windowHeight), initialX(x), initialY(y), isBallMoving(false){}


bool Ball::Update(Paddle& paddle, std::vector<Brick>& bricks) {
    if (isBallMoving) {
        // Update ball position
        x += velocityX;
        y += velocityY;

        // Check for collisions with window boundaries and paddle
        if (x - radius < 0 || x + radius > windowWidth) {
            velocityX = -velocityX; // Reverse horizontal velocity
        }
        if (y - radius < 0) {
            velocityY = -velocityY; // Reverse vertical velocity for top boundary
        }

        else if (x > paddle.getX() && x < paddle.getX() + paddle.getWidth() && y + radius > paddle.getY()) {
            velocityY = -velocityY; // Reverse vertical velocity for paddle
        }

        // Check for collision with bricks
        for (auto it = bricks.begin(); it != bricks.end();) {
            Brick& brick = *it;

            if (x + radius > brick.getX() && x - radius < brick.getX() + brick.getWidth() &&
                y + radius > brick.getY() && y - radius < brick.getY() + brick.getHeight()) {
                // Collision with brick
                velocityY = -velocityY; // Reverse
                brick.Hit();
                if (brick.getDurability() == 1 && brick.getIsGray()) {
                    brick.setIsGray(false);
                }
            }
            if (brick.IsBroken()) {
                it = bricks.erase(it);
            }
            else {
                ++it;
            }

    }
            // Check for collision with bottom
            if (y + radius > windowHeight) {
                setBallMoving(false);
                return false;
            }
    }
    return true;
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

    this->x = x;
}

void Ball::setY(int y) {

    this->y = y;
}

void Ball::setRadius(int radius) {
    this->radius = radius;
}

void Ball::setVelocityX(int velocityX) {
    this->velocityX = velocityX;
}

void Ball::setVelocityY(int velocityY) {
    this->velocityY = velocityY;
}

void Ball::setBallMoving(bool state) {
    this->isBallMoving = state;
}

int Ball::getX() const {
    return x;
}

int Ball::getY() const {
    return y;
}

int Ball::getRadius() const {
    return radius;
}

int Ball::getVelocityX() const{
    return this->velocityX;
}

int Ball::getVelocityY() const {
    return this->velocityY;
}

bool Ball::getBallState() const {
    return this->isBallMoving;
}

int Ball::getInitialX() const {
    return initialX;
}

int Ball::getInitialY() const {
    return initialY;
}