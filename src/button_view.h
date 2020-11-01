#ifndef BUTTON_VIEW_H_INCLUDED
#define BUTTON_VIEW_H_INCLUDED

#include <SDL.h>
#include "view.h"
#include <string>
#include <functional>

class ButtonView : public View {
    public:
        ButtonView(SDL_Rect buttonBoundary, std::string buttonText, SDL_Color buttonColor, std::function<void()> callback, int deltaX = 0, int deltaY = 0);
        ~ButtonView();
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        void handleClicks(SDL_MouseButtonEvent& mouseButtonEvent);
        void handleResize(SDL_Surface* newSurface);
    private:
        SDL_Rect buttonBoundary_;
        std::string buttonText_;
        SDL_Color buttonColor_;
        SDL_Color borderColor_;
        SDL_Surface* renderText;
        std::function<void()> callback_;
        int deltaX;
        int deltaY;
};

#endif // BUTTON_VIEW_H_INCLUDED
