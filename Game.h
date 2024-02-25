#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bricks.h"
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
    bool isGameWon();

    void LoadScoresFromFile();
    void InitBricks();
    void InitPaddle();
    void InitBall();
    void ResetPaddleAndBall();

    void RunGameLoop();
    void RenderStartScreen();
    void RenderGameOverScreen();
    void RenderGameWonScreen();
    void RenderTopScreenElements();

  
    int getWindowWidth() const;
    int getWindowHeight() const;

    int getLives() const;
    void setLives(int);

    int getScore() const;
    void setScore(int);

    void TogglePause();

    std::vector<Brick> getBricks() const;

private:
    SDL_Rect startButtonRect;
    SDL_Rect gameOverRect;
    SDL_Rect gameWonRect;
    SDL_Rect scoreRect;
    SDL_Rect pauseButtonRect;
    SDL_Rect xNummberOfLivesRect;

    bool gameStarted; 
    bool gameOver;
    bool gameWon;
    bool running;
    bool paused;

    SDL_Window* window;
    SDL_Renderer* renderer;
   
    int currentFrame;
    Paddle paddle;
    Ball ball;
    std::vector<Brick> bricks;
    FileHandler fileHandler;
    FontManager fontManager;
    int fontSize;
    int windowWidth;
    int windowHeight;
    int playerLives;
    int score;

    Mix_Chunk* startSound;
    Mix_Music* backgroundMusic;
};
