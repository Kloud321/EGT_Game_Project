#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "string"

class Scoreboard {
public:
    Scoreboard(int lives);
    void Render(SDL_Renderer* renderer);
    void UpdateScore(int points);
    void ResetLives();
    void LoseLife();
    bool IsGameOver() const;
    bool IsGameWon() const;
    int GetScore() const;
    int GetLives() const;
private:
    int score_;
    int lives_;
    bool isGameOver_;
    bool isGameWon_;
    TTF_Font* font_;
};
