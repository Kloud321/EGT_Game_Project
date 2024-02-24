#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bricks.h"
#include "Scoreboard.h"
#include "FileHandler.h"
#include "FontManager.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();
    bool Init(const char* title, int xpos, int ypos, int width, int height, int flags);
    void HandleEvents();
    void Update();
    void Render();

    bool IsRunning();
    void Clean();

    void InitStartButton();
    bool IsMouseOverStartButton(int, int);

    bool checkGameStarted() const;

    void InitBricks();
    void InitPaddleAndBall();

    void RunGameLoop();
    void RenderStartScreen();
  
    int getWindowWidth() const;
    int getWindowHeight() const;

    int getLives() const;
    void setLives(int);
 
private:
    SDL_Rect startButtonRect;
    bool gameStarted;  

    SDL_Window* window;
    SDL_Renderer* renderer;
   
    bool running;
    int currentFrame;
    Paddle paddle;
    Ball ball;
    std::vector<Brick> bricks;
    Scoreboard scoreboard;
    FileHandler fileHandler;
    FontManager fontManager;
    int fontSize;
    int windowWidth;
    int windowHeight;
    int playerLives;


};
