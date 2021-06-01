#include "fps_view.h"
#include "asset_manager.h"

#include <string>

using namespace std;

FrameRateView::FrameRateView(const View& parentView_)
    : parentView(parentView_), fpsTextSurface(nullptr) {}

FrameRateView::~FrameRateView() {
    if (fpsTextSurface != nullptr) {
        SDL_FreeSurface(fpsTextSurface);
    }
}

void FrameRateView::updateFps(double averageFps) {
    const AssetManager& am = getAssetManager();
    TTF_Font* font = am.getFont("gidole");
    string text = to_string(averageFps);
    if (fpsTextSurface != nullptr) {
        SDL_FreeSurface(fpsTextSurface);
    }
    fpsTextSurface = TTF_RenderUTF8_Shaded(font, text.c_str(),
                                           SDL_Color{255, 255, 255, 255}, SDL_Color{0, 0, 0, 0});
}

void FrameRateView::draw(SDL_Surface* screen) {
    // Rendering text in bottom left of parent view.
    SDL_Rect parentRect = parentView.boundary();
    SDL_Rect textRect {parentRect.x,
                       parentRect.y + parentRect.h - fpsTextSurface->h,
                       fpsTextSurface->w,
                       fpsTextSurface->h};
    SDL_BlitSurface(fpsTextSurface,
                    nullptr,
                    screen,
                    &textRect);
}
