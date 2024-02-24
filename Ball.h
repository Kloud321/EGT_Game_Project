#pragma once
#include <SDL.h>
#include <iostream>
#include <string>
#include "Paddle.h"
#include "Bricks.h"
#include <vector>

using std::cout;
using std::endl;

//forward declaration
class Game;

class Ball {
public:
    Ball(int x, int y, int radius, int velocityX, int velocityY, int windowWidth, int windowHeight);

    bool Update(Paddle& paddle, std::vector<Brick>&);

    void Render(SDL_Renderer* renderer);

    SDL_Rect GetRect();

    void ChangeDirectionX();
    void ChangeDirectionY();

    void setWidth(int);
    int getWidth() const;

    void setHeight(int);
    int getHeight() const;

    void setX(int);
    int getX() const;

    void setY(int);
    int getY() const;

    void setRadius(int);
    int getRadius() const;

    void setVelocityX(int);
    int getVelocityX() const;

    void setVelocityY(int);
    int getVelocityY() const;

    void setBallMoving(bool);
    bool getBallState() const;

    int getInitialX() const;
    int getInitialY() const;

private:
    int x;
    int y;
    int radius;
    int velocityX;
    int velocityY;
    int windowWidth;
    int windowHeight; 
    int initialX;
    int initialY;
    bool isBallMoving;
};
