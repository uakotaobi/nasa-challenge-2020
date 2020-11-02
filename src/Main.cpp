#include <iostream>
#include "SDL.h"
#include "button_view.h"
#include "SDL_ttf.h"
#include "menu_view.h"
#include "SDL_image.h"
#include "main_view.h"

int main() {
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 2;
    }

    // load support for the JPG and PNG image formats
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initialized = IMG_Init(flags);
    if ((initialized & flags) != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    const int width = 1800;

    const int height = 1050;

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    SDL_Surface* surf;
    surf = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (surf == NULL) {
        SDL_Log("SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
        return 1;
    }

    bool redraw;

    // Create views that user will see
    int currentView = 0;
    MenuView menuView(surf, currentView);
    MainView mainView(surf);

    while (currentView >= 0) {
        redraw = false;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // Window was closed
                    currentView = -1;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)                     {
                        if (currentView == 1) {
                            // Break out of main view.
                            currentView = 0;
                            redraw = true;
                        } else if (currentView == 0) {
                            // Break out of menu view (exit the program.)
                            currentView = -1;
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.clicks == 1) {
                        menuView.handleClicks(event.button);
                    }
                    redraw = true;
                    break;
                case SDL_WINDOWEVENT:
                    // Window is resized.
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        int newWidth = event.window.data1;
                        int newHeight = event.window.data2;
                        surf = SDL_CreateRGBSurfaceWithFormat(0, newWidth, newHeight, 32, SDL_PIXELFORMAT_RGBA32);
                        menuView.handleResize(surf);
                    }  else if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        // The window was exposed and should be repainted.
                        // std::cout << event.window.event << ": must redraw\n";
                        redraw = true;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    redraw = true;
                    break;
            }
        }

        if (redraw) {
            if (currentView == 0) {
                menuView.draw(surf);
            } else if (currentView == 1) {
                mainView.draw(surf);
            }

            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

            if (texture == NULL) {
                fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
                exit(1);
            }

            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            SDL_DestroyTexture(texture);
        }
        SDL_Delay(1000/30);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();


    std::cout << "Program finished.\n";
}
