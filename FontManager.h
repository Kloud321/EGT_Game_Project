#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class FontManager {
public:
    FontManager();
    ~FontManager();

    bool loadFont(const std::string& fontPath, int fontSize);
    void renderText(const std::string& text, SDL_Color textColor, SDL_Renderer* renderer, int x, int y);
    SDL_Texture* createTextTexture(const std::string& text, SDL_Color textColor, SDL_Renderer* renderer);
    void getTextSize(const std::string& text, SDL_Color textColor, int* width, int* height);

private:
    TTF_Font* font;
};

