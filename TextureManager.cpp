#include "TextureManager.h"

TextureManager* TextureManager::instance;

bool TextureManager::loadTexture(const char* fileName,
	std::string id, SDL_Renderer* ren) {
	SDL_Surface* tempSurface = IMG_Load(fileName);

	if (tempSurface == 0) {
		return false; // something went wrong
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
	SDL_FreeSurface(tempSurface);

	if (tex != 0) {
		textureMap[id] = tex;
		return true;
	}

	return false; // something went wrong
}

void TextureManager::drawTexture(std::string id, int x, int y,
	int width, int height,
	SDL_Renderer* ren, SDL_RendererFlip flip) {


	if (textureMap.find(id) == textureMap.end()) {
		std::cerr << "Texture with id: '" << id << "' is not found." << std::endl;
		return;

	}
	SDL_SetTextureBlendMode(textureMap[id], SDL_BLENDMODE_BLEND);

	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = srcRect.y = 0;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(ren, textureMap[id], &srcRect, &destRect, 0, 0, flip);
}


void TextureManager::clean() {
	for (auto& texture : textureMap) {
		SDL_DestroyTexture(texture.second);
	}
	textureMap.clear();
}
