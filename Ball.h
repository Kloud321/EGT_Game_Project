#include <SDL.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

class Ball {
public:
    Ball(int x, int y, int radius, int velocityX, int velocityY, int windowWidth, int windowHeight);
    void Update();
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

private:
    int x;
    int y;
    int radius;
    int velocityX;
    int velocityY;
    int windowWidth;
    int windowHeight;

};
