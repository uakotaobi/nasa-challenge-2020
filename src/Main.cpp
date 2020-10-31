#include <iostream>
#include "SDL.h"
#include "button_view.h"
#include "SDL_ttf.h"
#include "menu_view.h"

int main() {
  if (TTF_Init() == -1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    return 2;
  }
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
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

  bool done = false;

  // Create views that user will see
  MenuView menuView(surf, done);

  while(!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                // Window was closed
                done = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.clicks == 1) {
                    menuView.handleClicks(event.button);
                }
				break;
			case SDL_WINDOWEVENT:
				// Window is resized.
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					int newWidth = event.window.data1;
					int newHeight = event.window.data2;
					surf = SDL_CreateRGBSurfaceWithFormat(0, newWidth, newHeight, 32, SDL_PIXELFORMAT_RGBA32);
				}
				break;

        }
    }
    menuView.draw(surf);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

    if (texture == NULL) {
      fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
      exit(1);
    }

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);

    SDL_Delay(1000/30);
  }



  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();


  std::cout << "Program finished.\n";
}
