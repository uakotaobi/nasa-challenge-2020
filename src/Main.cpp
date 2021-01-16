#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "SDL.h"
#include "button_view.h"
#include "SDL_ttf.h"
#include "menu_view.h"
#include "SDL_image.h"
#include "main_view.h"
#include "vector.h"
#include "matrix.h"
#include "plane.h"
#include "grid.h"

using namespace std;

// Checking if the camera has left the grid.
// Returns the velocity vector that will be used for movement.
Vector detectCollision(Basis camera, Vector velocity, const Grid& grid) {
    auto planes = {
        grid.leftPlane(),
        grid.rightPlane(),
        grid.forwardPlane(),
        grid.backPlane()
    };

    Point futureLocation = camera.center + velocity;

    for (Plane p : planes) {
        if (p.whichSide(futureLocation) > 0) {
            // Outside of boundaries
            return Vector(0, 0, 0);
        }
    }
    return velocity;
}

void debugPrint() {
    double focalDistance = 60;
    SDL_Rect screenRect = {-100, -100, 200, 200};
    SDL_Rect viewPortRect = {0, 0, 1500, 1000};

    Vector you_x_axis(-1, 0, 1);
    Vector you_z_axis(1, 0, 1);
    Vector you_y_axis(0, 1, 0);

    you_x_axis = normalize(you_x_axis);
    you_y_axis = normalize(you_y_axis);
    you_z_axis = normalize(you_z_axis);

    Point you(0, 0, 4);
    Point that = you + you_x_axis * 100 - you_y_axis * 4 + you_z_axis * 2;
    std::vector<Point> points = {
        that,
        you - you_x_axis * 10 - you_y_axis * 1000000 - you_z_axis * 15,
        you - you_x_axis * 0 - you_y_axis * 0 - you_z_axis * 0
    };

    Plane p(you, you_z_axis);
    for (std::vector<Point>::iterator iter = points.begin(); iter != points.end(); iter++) {
        std::stringstream s;
        s << *iter;
        std::cout.width(40);
        // std::cout << std::left << s.str() << projectionMatrix(focalDistance, screenRect, viewPortRect) * (*iter) << "\n";
        std::cout << std::left << s.str() << cameraTransform(you_x_axis, you_y_axis, you_z_axis, you) * (*iter) << ",      " << p.whichSide(*iter) << "\n";

    }
}

int main() {
    // debugPrint();
    // return 0;

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

    const int width = 1200;
    const int height = 700;
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

    // Kinematic variables
    const double accelerationRate = 2;        // units/frame
    const double angularAccelerationRate = 1; // degrees/frame
    double currentTurningRate = 0;            // degrees/frame
    const double maxTurningRate = 1;          // degrees/frame
    Vector velocity(0, 0, 0);                 // current velocity
    const double maxVelocity = 50;            // units/frame
    const double frictionDecay = 0.85;        // %velocity per frame;
    const double turningFrictionDecay = 0.75; // %velocity per frame;

    mainView.getGrid().setHeightByFunction([] (double x_, double y_) {
        double x = x_ * 20 - 10;
        double y = y_ * 20 - 10;
        double z = sin(sqrt(x*x + y*y)) / (sqrt(x*x + y*y));
        return z;
    }, [] (double x_, double y_) {
        uint8_t x = static_cast<uint8_t>(x_ * 255);
        uint8_t y = static_cast<uint8_t>(y_ * 255);
        uint8_t squarert = static_cast<uint8_t>(sqrt(x_ * y_) * 255);
        return SDL_Color{x, y, squarert, 255};
    });

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
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        if (currentView == 1) {
                            // Break out of main view.
                            currentView = 0;
                            redraw = true;
                        } else if (currentView == 0) {
                            // Break out of menu view (exit the program.)
                            currentView = -1;
                        }
                    } else if (event.key.keysym.sym == SDLK_w) {
                        if (currentView == 1) {
                            velocity -= normalize(mainView.getCamera().axisZ) * accelerationRate;
                            if (velocity.magnitude() > maxVelocity) {
                                velocity = normalize(velocity) * maxVelocity;
                            }
                            redraw = true;
                        }
                    } else if (event.key.keysym.sym == SDLK_s) {
                        if (currentView == 1) {
                            velocity += normalize(mainView.getCamera().axisZ) * accelerationRate;
                            if (velocity.magnitude() > maxVelocity) {
                                velocity = normalize(velocity) * -maxVelocity;
                            }
                            redraw = true;
                        }
                    } else if (event.key.keysym.sym == SDLK_d) {
                        if (currentView == 1) {
                            currentTurningRate = std::min(currentTurningRate + angularAccelerationRate, maxTurningRate);
                            redraw = true;
                        }
                    } else if (event.key.keysym.sym == SDLK_a) {
                        if (currentView == 1) {
                            currentTurningRate = std::max(currentTurningRate - angularAccelerationRate, -maxTurningRate);
                            redraw = true;
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.clicks == 1 && currentView == 0) {
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

        // Handle camera movement
        Basis camera = mainView.getCamera();
        Vector momentaryVelocity = detectCollision(camera, velocity, mainView.getGrid());
        velocity = momentaryVelocity;
        camera.apply(translationMatrix(momentaryVelocity) * rotationMatrix(camera.center, camera.center + camera.axisY, currentTurningRate));
        mainView.setCamera(camera);
        velocity *= frictionDecay;
        currentTurningRate *= turningFrictionDecay;
        // Animate sliding / turning
        if (velocity.magnitude() > 0 || abs(currentTurningRate) > 0) {
            redraw = true;
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

            /* // Warning: temporary!
            // Quit after rendering the first frame.
            if (currentView == 1) {
                break;
            } */
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
