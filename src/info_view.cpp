#include "info_view.h"
#include "asset_manager.h"
#include "SDL.h"

InfoView::InfoView(SDL_Rect infoBoundary, int deltaX, int deltaY) {
    std::string infoText = "Info";
    boundaryInfoView = infoBoundary;
    const AssetManager& am = getAssetManager();
    TTF_Font* infoFont = am.getFont("gidole");
    renderText = TTF_RenderUTF8_Shaded(infoFont, infoText.c_str(), SDL_Color{255, 255, 255, 255}, SDL_Color{255, 61, 64});
}

void InfoView::draw(SDL_Surface* screen) {
    SDL_FillRect(screen, &boundaryInfoView, SDL_MapRGB(screen->format, 255, 61, 64));

    // Draw the info text.
    SDL_Rect rec = SDL_Rect{
      boundaryInfoView.x + boundaryInfoView.w / 2 - renderText->w / 2 ,
      boundaryInfoView.y + boundaryInfoView.h / 8 - renderText->h / 2,
      0 /* ignored */,
      0 /* ignored */
    };
    SDL_BlitSurface(renderText,
                    nullptr,
                    screen,
                    &rec);
}

SDL_Rect InfoView::boundary() const {
    return boundaryInfoView;
}
