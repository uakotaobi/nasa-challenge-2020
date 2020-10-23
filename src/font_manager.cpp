#include "font_manager.h"
#include <stdexcept>

FontManager::FontManager() {
      TTF_Font* font = TTF_OpenFont("fonts/Gidole-Regular.ttf", 16);
      if (!font) {
          printf("TTF_OpenFont: %s\n", TTF_GetError());
      }
      fontRegistry["gidole"] = font;

      font = TTF_OpenFont("fonts/Gidolinya-Regular.otf", 16);
      if (!font) {
          printf("TTF_OpenFont: %s\n", TTF_GetError());
      }
      fontRegistry["gidolinya"] = font;
}

const FontManager& getFontManager() {
      static FontManager f;
      return f;
}

const TTF_Font* FontManager::getFont(std::string fontName) const {
      if (fontRegistry.find(fontName) != fontRegistry.end()) {
          throw std::runtime_error("Could not find " + fontName);
      }
      return fontRegistry.find(fontName)->second;
}
