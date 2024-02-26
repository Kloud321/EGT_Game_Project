#include "Ball.h"
#include "Game.h"

Ball::Ball(int x, int y, int radius, int velocityX, int velocityY, int windowWidth, int windowHeight)
    : x(x), y(y), radius(radius), velocityX(velocityX), velocityY(velocityY), windowWidth(windowWidth),
    windowHeight(windowHeight), initialX(x), initialY(y), isBallMoving(false){}

bool Ball::Update(Paddle& paddle, std::vector<Brick>& bricks, int& score) {

    if (isBallMoving) {
        // Update ball position
        x += velocityX;
        y += velocityY;

        // Check for collisions with window boundaries and paddle
        if (x - radius <= 0 || x + radius >= windowWidth) {
            velocityX = -velocityX; // Reverse horizontal velocity
        }
        if (y - radius < 0) {
            velocityY = -velocityY; // Reverse vertical velocity for top boundary
        }
                //right sBall  right sPaddle    left sBall          right sPaddle                 bottom ball   top paddle      top ball            bottom paddle
        //else if (x + radius >= paddle.getX() && x - radius <= paddle.getX() + paddle.getWidth() && y + radius > paddle.getY() && y - radius < paddle.getY() + paddle.getHeight()) {
        //    // Change ball direction
        //    velocityY = -velocityY;
        //}
        
        else if (x + radius >= paddle.getX() && x - radius <= paddle.getX() + paddle.getWidth() && y + radius > paddle.getY() && y - radius < paddle.getY() + paddle.getHeight()) {
            // Check if the collision is with the top side of the paddle
            if (y - radius < paddle.getY()) {
                // Reverse the vertical velocity for a bounce upwards
                velocityY = -velocityY;

                y = paddle.getY() - radius;
            }
            else {
                // Calculate the distance between the center of the ball and the center of the paddle
                int paddleCenterX = paddle.getX() + paddle.getWidth() / 2;
                int ballDistanceFromPaddleCenter = x - paddleCenterX;
                int speed = 6;

                // Calculate the normalized distance to determine the direction of the bounce
                double normalizedDistance = static_cast<double>(ballDistanceFromPaddleCenter) / (paddle.getWidth() / 2);

                // Determine bounce direction based on the normalized distance
                if (normalizedDistance < 0) {
                    // Left side collision: bounce towards the left
                    velocityX = -speed; // Set horizontal velocity to the left
                }
                else {
                    // Right side collision: bounce towards the right
                    velocityX = speed; // Set horizontal velocity to the right
                }

                // Reverse vertical velocity for other collisions
                velocityY = -speed; // Reverse vertical velocity
            }
        }


        // Check for collision with bricks
        for (auto it = bricks.begin(); it != bricks.end();) {
            Brick& brick = *it;

            //if (x + radius > brick.getX() && x - radius < brick.getX() + brick.getWidth() &&
            //    y + radius > brick.getY() && y - radius < brick.getY() + brick.getHeight()) {
            //    // Collision with brick
                //velocityY = -velocityY; // Reverse

            if (x + radius > brick.getX() && x - radius < brick.getX() + brick.getWidth() &&
                y + radius > brick.getY() && y - radius < brick.getY() + brick.getHeight()) {
                // Check if the collision is with the top side of the brick
                if (y - radius < brick.getY()) {
                    // Reverse the vertical velocity for a bounce upwards
                    velocityY = -velocityY;
                    // Adjust the ball's position to be just above the brick
                    y = brick.getY() - radius;
                }
                else if (y + radius > brick.getY() + brick.getHeight()) {
                    // Reverse the vertical velocity for a bounce downwards
                    velocityY = -velocityY;
                    // Adjust the ball's position to be just below the brick
                    y = brick.getY() + brick.getHeight() + radius;
                }

                // Check if the collision is with the left side of the brick
                if (x - radius < brick.getX() && x > brick.getX()) {
                    // Reverse the horizontal velocity for a bounce to the left
                    velocityX = -velocityX;
                    // Adjust the ball's position to be just left of the brick
                    x = brick.getX() - radius;
                }

                // Check if the collision is with the right side of the brick
                if (x - radius < brick.getX() + brick.getWidth() && x > brick.getX() + brick.getWidth()) {
                    // Reverse the horizontal velocity for a bounce to the right
                    velocityX = -velocityX;
                    // Adjust the ball's position to be just right of the brick
                    x = brick.getX() + brick.getWidth() + radius;
                }
                brick.Hit();
                score += 1;
                
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
}

SDL_Rect Ball::GetRect(){
    return { x - radius, y - radius, radius * 2, radius * 2 };
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