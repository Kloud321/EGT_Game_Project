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


    void update();

    void setWidth(int);
    int getWidth() const;

    void setHeight(int);
    int getHeight() const;

    void setX(int);
    int getX() const;

    void setY(int);
    int getY() const;

private:
    int x;
    int y;
    int width;
    int height;
};
