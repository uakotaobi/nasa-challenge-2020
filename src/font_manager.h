#ifndef FONT_MANAGER_H_INCLUDED
#define FONT_MANAGER_H_INCLUDED

#include <map>
#include <string>
#include "SDL_ttf.h"

class FontManager {
    private:
        FontManager();
        std::map<std::string, const TTF_Font*> fontRegistry;
    public:
        friend const FontManager& getFontManager();
        const TTF_Font* getFont(std::string fontName) const;
};









#endif // FONT_MANAGER_H_INCLUDED
