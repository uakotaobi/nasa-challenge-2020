#ifndef MENU_VIEW_H_INCLUDED
#define MENU_VIEW_H_INCLUDED

#include <SDL.h>
#include "view.h"
#include "button_view.h"

// MenuView is responsible for the initial menu when the program opens
class MenuView : public View {
    public:
        MenuView(SDL_Surface* surf, int& currentView);
        ~MenuView();
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        void handleClicks(SDL_MouseButtonEvent& mouseButtonEvent);
        void handleResize(SDL_Surface* newSurface);

    private:
        ButtonView startButton;
        ButtonView quitButton;
        SDL_Surface* heroicImage;
};


#endif // MENU_VIEW_H_INCLUDED
