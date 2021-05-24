#include "fps_view.h"
#include "asset_manager.h"

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
    fpsTextSurface = TTF_RenderUTF8_Shaded(font, )
}