#ifndef INFO_VIEW_H_INCLUDED
#define INFO_VIEW_H_INCLUDED

#include "view.h"

class InfoView : public View {
    public:
        InfoView(SDL_Rect infoBoundary, int deltaX, int deltaY);
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        // void handleResize(SDL_Surface* screen);
    private:
        SDL_Rect boundaryInfoView;
};




#endif // INFO_VIEW_H_INCLUDED
