#pragma once
#include <SDL.h>
#include <iostream>
#include <string>
#include "Paddle.h"

using std::cout;
using std::endl;

//forward declaration
class Game;

class Ball {
public:
    Ball(int x, int y, int radius, int velocityX, int velocityY, int windowWidth, int windowHeight);

    void Update(Paddle& paddle, Game& game);
 
    void Render(SDL_Renderer* renderer);

    SDL_Rect GetRect();

    void ChangeDirectionX();
    void ChangeDirectionY();

    void setWidth(int);
    int getWidth();

    void setHeight(int);
    int getHeight();

    void setX(int);
    int getX();

    void setY(int);
    int getY();

    void setRadius(int);
    int getRadius();

    void setVelocityX(int);
    int getVelocityX();

    void setVelocityY(int);
    int getVelocityY();


    void setBallMoving(bool);
    bool getBallState() const;

    //void setInitialPosition(int, int);

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
