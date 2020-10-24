#include "font_manager.h"
#include <stdexcept>

// Font manager constructor
FontManager::FontManager() {
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

const FontManager& getFontManager() {
      static FontManager f;
      return f;
}

TTF_Font* FontManager::getFont(std::string fontName) const {
      if (fontRegistry.find(fontName) == fontRegistry.end()) {
          throw std::runtime_error("Could not find " + fontName);
      }
      return fontRegistry.find(fontName)->second;
}
