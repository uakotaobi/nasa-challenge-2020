#ifndef FONT_MANAGER_H_INCLUDED
#define FONT_MANAGER_H_INCLUDED

#include <map>
#include <string>
#include "SDL_ttf.h"

class FontManager {
    private:
        FontManager();
        std::map<std::string, TTF_Font*> fontRegistry;
    public:
        friend const FontManager& getFontManager();
        TTF_Font* getFont(std::string fontName) const;
};

extern const FontManager& getFontManager();







#endif // FONT_MANAGER_H_INCLUDED
