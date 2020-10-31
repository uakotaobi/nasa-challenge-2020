#include "asset_manager.h"
#include <stdexcept>
#include "SDL_image.h"

// Font manager constructor
AssetManager::AssetManager() {
      TTF_Font* font = TTF_OpenFont("../fonts/Gidole-Regular.ttf", 48);
      if (!font) {
          printf("TTF_OpenFont: %s\n", TTF_GetError());
      }
      fontRegistry["gidole"] = font;

      font = TTF_OpenFont("../fonts/Gidolinya-Regular.otf", 48);
      if (!font) {
          printf("TTF_OpenFont: %s\n", TTF_GetError());
      }
      fontRegistry["gidolinya"] = font;
}

const AssetManager& getAssetManager() {
      static AssetManager f;
      return f;
}

TTF_Font* AssetManager::getFont(std::string fontName) const {
      if (fontRegistry.find(fontName) == fontRegistry.end()) {
          throw std::runtime_error("Could not find " + fontName);
      }
      return fontRegistry.find(fontName)->second;
}

SDL_Surface* AssetManager::getImage(std::string imageFileName) const {
      std::string imagePath = "../images/" + imageFileName;
      SDL_Surface* image = IMG_Load(imagePath.c_str());
      if (!image) {
          throw std::runtime_error(IMG_GetError());
      }
      return image;
}
