#pragma once
#include <SDL.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

class Paddle {
public:

    Paddle(int x, int y, int width, int height);
    void MoveLeft();
    void MoveRight();
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect();

    void setWidth(int);
    int getWidth();

    void setHeight(int);
    int getHeight();

    void setX(int);
    int getX();

    void setY(int);
    int getY();

private:
    int x;
    int y;
    int width;
    int height;
};
