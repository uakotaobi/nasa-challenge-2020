#ifndef ASSET_MANAGER_H_INCLUDED
#define ASSET_MANAGER_H_INCLUDED

#include <map>
#include <string>
#include "SDL_ttf.h"

class AssetManager {
    private:
        AssetManager();
        std::map<std::string, TTF_Font*> fontRegistry;
    public:
        friend const AssetManager& getAssetManager();
        TTF_Font* getFont(std::string fontName) const;
        SDL_Surface* getImage(std::string imageFileName) const;
};

extern const AssetManager& getAssetManager();

#endif // ASSET_MANAGER_H_INCLUDED
