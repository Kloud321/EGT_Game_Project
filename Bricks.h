#pragma once
#include <SDL.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

class Brick {
public:
    Brick(int x, int y, int width, int height, bool isGray);
    void Render(SDL_Renderer* renderer);
    bool IsBroken() const;
    void Hit();
    SDL_Rect GetRect() const;

private:
    int x;
    int y;
    int width;
    int height;
    bool isGray;
    int durability;
};
