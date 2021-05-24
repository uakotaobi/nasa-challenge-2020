#ifndef FPS_VIEW_H_INCLUDED
#define FPS_VIEW_H_INCLUDED

#include "view.h"

class FrameRateView {
    public:
        // Construct a frame rate view that will be 
        // displayed in the corner of a parent view.
        FrameRateView(const View& parentView_);
        ~FrameRateView();

        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        
        // Tells the FrameRateView to update the view with the average fps.
        void updateFps(double averageFps);

    private:
        const View& parentView;
        SDL_Surface* fpsTextSurface;
};



#endif // FPS_VIEW_H_INCLUDED