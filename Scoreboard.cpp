#include "Scoreboard.h"

Scoreboard::Scoreboard(int lives) : score_(0), lives_(lives), isGameOver_(false), isGameWon_(false), font_(TTF_OpenFont("arial.ttf", 24)) {}

void Scoreboard::Render(SDL_Renderer* renderer) {
    std::string text = "Lives: " + std::to_string(lives_) + " | Score: " + std::to_string(score_);
    SDL_Surface* surface = TTF_RenderText_Solid(font_, text.c_str(), { 255, 255, 255 });
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = { 10, 10, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Scoreboard::UpdateScore(int points) {
    score_ += points;
}

void Scoreboard::ResetLives() {
    lives_ = 3;
}

void Scoreboard::LoseLife() {
    lives_--;
    if (lives_ <= 0) {
        isGameOver_ = true;
    }
}

bool Scoreboard::IsGameOver() const {
    return isGameOver_;
}

bool Scoreboard::IsGameWon() const {
    return isGameWon_;
}

int Scoreboard::GetScore() const {
    return score_;
}

int Scoreboard::GetLives() const {
    return lives_;
}
