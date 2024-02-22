// FontManager.cpp
#include "FontManager.h"
#include <iostream>

FontManager::FontManager() : font(nullptr) {}

FontManager::~FontManager() {
    if (font) {
        TTF_CloseFont(font);
    }
}

bool FontManager::loadFont(const std::string& fontPath, int fontSize) {
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void FontManager::renderText(const std::string& text, SDL_Color textColor, SDL_Renderer* renderer, int x, int y) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    SDL_Rect dstRect = { x, y, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

SDL_Texture* FontManager::createTextTexture(const std::string& text, SDL_Color textColor, SDL_Renderer* renderer) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(textSurface);
    return textTexture;
}


void FontManager::getTextSize(const std::string& text, SDL_Color textColor, int* width, int* height) {
    //We check if the font is initialized
    if (!font) {
        std::cerr << "Font not initialized!" << std::endl;
        *width = 0;
        *height = 0;
        return;
    }

    // We get the dimensions of the rendered text
    if (TTF_SizeText(font, text.c_str(), width, height) != 0) {
        std::cerr << "Failed to get text size: " << TTF_GetError() << std::endl;
        *width = 0;
        *height = 0;
        return;
    }
}