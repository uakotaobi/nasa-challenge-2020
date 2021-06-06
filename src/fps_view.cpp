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
    text = text.substr(0, text.find(".") + 2);
    if (fpsTextSurface != nullptr) {
        SDL_FreeSurface(fpsTextSurface);
    }
    fpsTextSurface = TTF_RenderUTF8_Shaded(font, text.c_str(),
                                           SDL_Color{255, 255, 255, 255}, SDL_Color{0, 0, 0, 0});
}

void FrameRateView::draw(SDL_Surface* screen) {
    SDL_Rect textRect = boundary();
    SDL_BlitSurface(fpsTextSurface,
                    nullptr,
                    screen,
                    &textRect);
}

// Define rectangle in bottom left of the parent view.
SDL_Rect FrameRateView::boundary() const {
    SDL_Rect parentRect = parentView.boundary();
    SDL_Rect textRect {parentRect.x,
                       parentRect.y + parentRect.h - fpsTextSurface->h,
                       fpsTextSurface->w,
                       fpsTextSurface->h};
    return textRect;
}



