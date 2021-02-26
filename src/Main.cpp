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
#include "matrix.h"
#include "plane.h"
#include "grid.h"
#include "common.h"
#include "vector.h"

using namespace std;

Point getFloor(Point cameraCenter, const Grid& moonGrid);

// Checking if the camera has left the grid.
// Returns the velocity vector that will be used for movement.
Vector detectCollision(Basis camera, Vector velocity, const Grid& grid,
                       double heightFromFloor, double gravitationalAcceleration) {
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

// Returns a vector that is ALWAYS parallel to the grid's axisY.
// If this vector is in the same direction as the grid's axisY then you are bouncing upward
// If this vector is in the opposite direction from the grid's axisY then you are falling downwards
// Otherwise, you are standing still (returns 0 vector)
Vector fallingVector(Basis camera, Vector verticalMotion, const Grid& grid,
                     double heightFromFloor, double gravitationalAcceleration) {

     Vector finalResult;
     Point groundPoint = getFloor(camera.center, grid);
     // The groundPlane is ALWAYS parallel to the grid.gridPlane().
     Plane groundPlane(groundPoint, grid.system().axisY);
     Point footPoint = camera.center - normalize(grid.system().axisY) * heightFromFloor;

     // Did our foot fall through the floor?
     if (groundPlane.whichSide(footPoint) < -epsilon) {
         // Yes it did- BOUNCE.
         const double bounceDecay = 0.75;
         finalResult = bounceDecay * groundPlane.reflection(verticalMotion);
         // Make sure we end up above the groundPlane.
         finalResult += groundPoint - footPoint;
         cout << "Units below groundPlane: " << distance(footPoint, groundPoint) << "\n";
     } else if (groundPlane.whichSide(footPoint) > epsilon) {
         // Fall.
         finalResult = -normalize(grid.system().axisY) * gravitationalAcceleration;
         Point p = footPoint + finalResult;
         if (groundPlane.whichSide(p) < 0) {
             finalResult = (groundPoint - footPoint);
         }
     }
     return finalResult;
}

// Reorients the camera so its vertical direction is equal to the absolute Y-axis
Basis unRollCamera(Vector absoluteAxisY, Basis camera) {
    double thetaCamera;
    double thetaAbsoluteAxisY;
    if (abs(camera.axisY.x) < epsilon && abs(camera.axisY.y) < epsilon) {
        // camera.axisY just is parallel to the absolute z axis.
        thetaCamera = atan2(camera.axisY.y, camera.axisY.z) * rad_to_deg;
        thetaAbsoluteAxisY = atan2(absoluteAxisY.y, absoluteAxisY.z) * rad_to_deg;
    } else {
        // This is our normal case.
        thetaCamera = atan2(camera.axisY.y, camera.axisY.x) * rad_to_deg;
        thetaAbsoluteAxisY = atan2(absoluteAxisY.y, absoluteAxisY.x) * rad_to_deg;
    }
    // std::cout << "thetaCamera: " << thetaCamera << ", ";
    // std::cout << "thetaAbsoluteAxisY: " << thetaAbsoluteAxisY << "\n";

    Matrix m = rotationMatrix(camera.center, camera.center + camera.axisZ, thetaCamera - thetaAbsoluteAxisY);
    camera.apply(m);
    return camera;
}

// Gets the floor point coordinate beneath camera (debugging)
Point getFloor(Point cameraCenter, const Grid& moonGrid) {
    auto uvh = moonGrid.gridLocation(cameraCenter);
    double u = std::get<0>(uvh);
    double v = std::get<1>(uvh);
    Point p = moonGrid.findFloor(u, v);
    return p;
}

double calculateAbsoluteElevation(Vector absoluteAxisY, Vector cameraDirection) {
     absoluteAxisY = normalize(absoluteAxisY);
     cameraDirection = normalize(cameraDirection);
     return acos(dotProduct(absoluteAxisY, cameraDirection)) * rad_to_deg;
}

void debugPrint() {
    Basis standard;
    double pitch = 0;
    double yaw = 0;
    double roll = 0;
    Vector i(1, 0, 0);
    Vector j(0, 1, 0);
    Vector k(0, 0, 1);
    standard.apply(translationMatrix(Vector(0, 100, -400)));
    Matrix m = eulerRotationMatrix(standard, 147.5, 60.5, 0);
    standard.apply(m);
    std::cout << "----- \n" << m << "basis form: " << standard << "\n";
    m = eulerRotationMatrix(standard, 0, 0.25, 0);
    standard.apply(m);
    std::cout << "----- \n" << m << "basis form: " << standard << "\n";
    m = eulerRotationMatrix(standard, -23.75, -21, 0);
    standard.apply(m);
    std::cout << "----- \n" << m << "basis form: " << standard << "\n";
    //std::cout << eulerRotationMatrix(standard, yaw, pitch, roll);

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
    const double framesPerSecond = 20;
    const double timeFactor = framesPerSecond/30; // This is a unitless constant that makes it so that 60 fps (or any other amount) has the same physics as 30 fps
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
    int previousMouseX = -1;
    int previousMouseY = -1;
    SDL_GetMouseState(&previousMouseX, &previousMouseY);
    double thetaTilt = 0;               // Camera tilt for the current frame (degrees)
    double thetaAzimuth = 0;            // Camera rotation for the current frame (degrees)
    const double pixelsToDegrees = .75;   // Mouse's pixel movement to rotation degrees ratio

    // Create views that user will see
    int currentView = 0;
    MenuView menuView(surf, currentView);
    MainView mainView(surf);

    // Kinematic variables
    const double accelerationRate = 2;                           // units/frame
    const double angularAccelerationRate = 1 / timeFactor;       // degrees/frame
    double currentTurningRate = 0;                               // degrees/frame
    const double maxTurningRate = 1 * timeFactor;                // degrees/frame
    Vector velocity(0, 0, 0);                                    // current velocity
    Vector verticalMotion(0, 0, 0);                              // gravity/bounce vector
    const double maxVelocity = 50 * timeFactor;                  // units/frame
    const double frictionDecay = 0.85;                           // %velocity per frame
    const double turningFrictionDecay = 0.75;                    // %velocity per frame
    const double gravitationalAcceleration = 9.8;                // units per second squared
    const double heightFromFloor = 10;                           // height of the avatar in units

    // sombrero. Ole!
    mainView.getGrid().setHeightByFunction([&mainView] (double x_, double y_) {
        double x = x_ * 20 - 10;
        double y = y_ * 20 - 10;
        double z = sin(sqrt(x*x + y*y)) / (sqrt(x*x + y*y));
        z = z * mainView.getGrid().cellSize() * sqrt(mainView.getGrid().rows() * mainView.getGrid().columns()) * .5;
        // return 0;
        return z;
    }, [] (double x_, double y_) {
        uint8_t x = static_cast<uint8_t>(x_ * 255);
        uint8_t y = static_cast<uint8_t>(y_ * 255);
        uint8_t squarert = static_cast<uint8_t>(sqrt(x_ * y_) * 255);
        return SDL_Color{x, y, squarert, 255};
    });

    while (currentView >= 0) {
        redraw = false;
        thetaTilt = 0;
        thetaAzimuth = 0;
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
                            velocity += normalize(mainView.getCamera().axisZ) * accelerationRate;
                            if (velocity.magnitude() > maxVelocity) {
                                velocity = normalize(velocity) * maxVelocity;
                            }
                            redraw = true;
                        }
                    } else if (event.key.keysym.sym == SDLK_s) {
                        if (currentView == 1) {
                            velocity -= normalize(mainView.getCamera().axisZ) * accelerationRate;
                            if (velocity.magnitude() > maxVelocity) {
                                velocity = normalize(velocity) * -maxVelocity;
                            }
                            redraw = true;
                        }
                    } else if (event.key.keysym.sym == SDLK_a) {
                        if (currentView == 1) {
                            currentTurningRate = std::min(currentTurningRate + angularAccelerationRate, maxTurningRate);
                            redraw = true;
                        }
                    } else if (event.key.keysym.sym == SDLK_d) {
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
                        redraw = true;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (currentView != 0) {
                        double deltaX = event.button.x - previousMouseX;
                        double deltaY = event.button.y - previousMouseY;
                        thetaTilt = (deltaY) * pixelsToDegrees;
                        thetaAzimuth = (deltaX) * pixelsToDegrees;

                        double currentElevation = calculateAbsoluteElevation(mainView.getGrid().system().axisY, mainView.getCamera().axisZ);

                        previousMouseX = event.button.x;
                        previousMouseY = event.button.y;

                    }
                    redraw = true;
                    break;
            }
        } // End of event processing.

        // Handle camera movement
        Basis camera = mainView.getCamera();
        Vector momentaryVelocity = detectCollision(camera, velocity, mainView.getGrid(), heightFromFloor, gravitationalAcceleration);
        velocity = momentaryVelocity;
        camera.apply(translationMatrix(momentaryVelocity));

        // Vertical motion is handled seperatly from lateral motion
        verticalMotion = fallingVector(camera, verticalMotion, mainView.getGrid(), heightFromFloor, gravitationalAcceleration);
        camera.apply(translationMatrix(verticalMotion));

        // DANGER WILL ROBINSON: GIMBAL LOCK
        // This prevents gimbal lock by stopping you from tilting to 180 or 0 degrees like to the Grid's z axis
        double currentElevation = calculateAbsoluteElevation(mainView.getGrid().system().axisY, camera.axisZ);
        const double maxDeviationFromHorizon = 10;
        if (currentElevation + thetaTilt >= 90 + maxDeviationFromHorizon) {
            thetaTilt = 90 + maxDeviationFromHorizon - currentElevation;
        } else if (currentElevation + thetaTilt <= 90 - maxDeviationFromHorizon) {
            thetaTilt = 90 - maxDeviationFromHorizon - currentElevation;
        }

        camera.apply(rotationMatrix(camera.center, camera.center + mainView.getGrid().system().axisY, currentTurningRate));
        // camera.apply(eulerRotationMatrix(camera, thetaAzimuth * 1, thetaTilt * 1, 0));
        camera.axisX = normalize(camera.axisX);
        camera.axisY = normalize(camera.axisY);
        camera.axisZ = normalize(camera.axisZ);
        camera = unRollCamera(mainView.getGrid().system().axisY, camera);


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
        SDL_Delay(1000/framesPerSecond);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();


    std::cout << "Program finished.\n";
}
