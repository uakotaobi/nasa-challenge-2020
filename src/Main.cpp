#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

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
#include "quaternion.h"
#include "render.h"

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
    Point correctedLocation = camera.center;
    Vector correctedVector = velocity;



    for (Plane p : planes) {
        double distanceFromPlane = p.distance(correctedLocation);
        if (distanceFromPlane > 0) {
            // Force correctedLocation to be inbounds for this plane.
            correctedLocation -= normalize(p.normalVector()) * distanceFromPlane;
            continue;
        }
        Point futureLocation = camera.center + correctedVector;
        if (p.whichSide(futureLocation) > 0) {
            // Our future location is out of bounds
            if (abs(p.whichSide(camera.center)) < epsilon) {
                // We are already on the wall - Slide
                correctedVector = p.projection(correctedVector);
            } else {
                // Far from wall, but about to move past it, so we need to move to the wall, then slide;
                auto intersection = p.pointOfIntersection(camera.center, futureLocation);
                if (intersection) {
                    correctedVector = *intersection - camera.center;
                }
            }
        }
    }
    if ((correctedLocation - camera.center).magnitude() > epsilon) {
        // We had to adjust our position because we were out of bounds.
        return correctedLocation - camera.center;
    }


    velocity = correctedVector;
    return velocity;
}

// Returns a vector that is ALWAYS parallel to the grid's axisY.
// If this vector is in the same direction as the grid's axisY then you are bouncing upward
// If this vector is in the opposite direction from the grid's axisY then you are falling downwards
// Otherwise, you are standing still (returns 0 vector)
Vector fallingVector(Basis& camera, Vector verticalMotion, const Grid& grid,
                     double heightFromFloor, double gravitationalAcceleration) {

     Vector finalResult;
     const Vector up = normalize(grid.system().axisY);
     const Vector down = -up;
     const double falling_epsilon = 0.01;
     const double bounceDecay = 0.50;
     Point groundPoint = getFloor(camera.center, grid);

     // The groundPlane is ALWAYS parallel to the grid.gridPlane().
     Plane groundPlane(groundPoint, up);

     Point footPoint = camera.center + down * heightFromFloor;

     // Are we too close to ground and not boosting?
     if (abs(groundPlane.whichSide(footPoint)) < falling_epsilon) {
         const double dot = dotProduct(normalize(verticalMotion), down);
         if (dot > 0 && dot <= 1) {
             // We're falling, and we're on the ground, so stop falling.
         return Vector{0, 0, 0};
         }
         // We're boosting, so keep doing that.
         return verticalMotion;
     } else if (groundPlane.whichSide(footPoint) < 0) {
         // Our feet went through the pavement.
         finalResult = groundPoint - footPoint;
     } else {
         // We are too far above the ground, so we need to fall.
         Vector putativeFallingVector = verticalMotion + down * gravitationalAcceleration;
         Point predictedFootPoint = footPoint + putativeFallingVector;
         if (groundPlane.whichSide(predictedFootPoint) < 0) {
             // Our predicted location after we fall will be through the floor,
             // so we need to stop at the floor, and bounce
             camera.center = groundPoint + up * heightFromFloor;
             finalResult = verticalMotion.magnitude() * up * bounceDecay;
         } else {
             finalResult = putativeFallingVector;
         }
     }
     return finalResult;
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

// Returns a rotation matrix that reorients the given Basis so that its axisY
// points "up" (i.e., so its axisY is parallel to absoluteAxisY.)
//
//              AbsoluteAxisY
//           |   ,'
// j (y-axis)|   |
//           |  ,'
//           |  |
//           | ,'
//           | |        __..--'"
//           |,'__..--'"   camera.axisY
// ----------+------------------> i (x-axis)
//           |
//
// atan2(camera.axisY.y, camera.axisY.x) is the angle between the camera's YZ
// plane and the true XZ plane.
//
// atan2(camera.absoluteAxisY.y, camera.absoluteAxisX.x) is the angle between
// the absolute (read: grid)'s YZ plane and the true XZ plane.
//
// Their difference is how much we need to rotate along the camera.axisZ in
// order to reorient ourselves.
Matrix correctVerticalOrientation(Vector absoluteAxisY, Basis camera) {
    double thetaCamera = atan2(camera.axisY.y, camera.axisY.x);
    double thetaAbsolute = atan2(absoluteAxisY.y, absoluteAxisY.x);
    const double RADIANS_TO_DEGREES = 180 / 3.141593;
    return rotationMatrix(camera.center,
                          camera.center + camera.axisZ,
                          -RADIANS_TO_DEGREES * (thetaAbsolute - thetaCamera));
}

void debugPrint() {
    // Basis standard;
    // double pitch = 0;
    // double yaw = 0;
    // double roll = 0;
    // Vector i(1, 0, 0);
    // Vector j(0, 1, 0);
    // Vector k(0, 0, 1);
    // standard.apply(translationMatrix(Vector(0, 100, -400)));
    // Matrix m = eulerRotationMatrix(standard, 147.5, 60.5, 0);
    // standard.apply(m);
    // std::cout << "----- \n" << m << "basis form: " << standard << "\n";
    // m = eulerRotationMatrix(standard, 0, 0.25, 0);
    // standard.apply(m);
    // std::cout << "----- \n" << m << "basis form: " << standard << "\n";
    // m = eulerRotationMatrix(standard, -23.75, -21, 0);
    // standard.apply(m);
    // std::cout << "----- \n" << m << "basis form: " << standard << "\n";
    // //std::cout << eulerRotationMatrix(standard, yaw, pitch, roll);
    Point p1(-13, 0.13, 0);
    Point p2(598, -291, 0);
    Point p3(23, 56, 0);
    Plane plane(p1, p2, p3);
    std::cout.precision(12);
    std::cout << "plane equation: " << plane.A << "x + " << plane.B << "y + " << plane.C << "z + " << plane.D << "= 0 \n";

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
    const double framesPerSecond = 30;
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
    double yawDeg = 0;    // Rotation with respect to absolute Y axis in degrees
    double pitchDeg = 0;  // Rotation with respect to absolute X axis in degrees
    double rollDeg = 0;   // Rotation with respect to absolute Z axis in degrees
    const double pixelsToDegrees = .35;   // Mouse's pixel movement to rotation degrees ratio

    map<int, bool> pressedKeys;

    // Create views that user will see
    int currentView = 0;
    MenuView menuView(surf, currentView);
    MainView mainView(surf);

    // Kinematic variables
    const double accelerationRate = 2;                              // units/frame
    const double angularAccelerationRate = 1 / timeFactor;          // degrees/frame
    double currentTurningRate = 0;                                  // degrees/frame
    const double maxTurningRate = 3 * timeFactor;                   // degrees/frame
    Vector velocity(0, 0, 0);                                       // current velocity
    Vector verticalMotion(0, 0, 0);                                 // gravity/bounce vector
    const double maxVelocity = 50 * timeFactor;                     // units/frame
    const double frictionDecay = 0.85;                              // %velocity per frame
    const double turningFrictionDecay = 0.75;                       // %velocity per frame
    const double earthGravityFudgeFactor = 1.0;                     // unitless
    const double gravitationalAcceleration = 9.8 / framesPerSecond * earthGravityFudgeFactor; // units per second^2
    const double heightFromFloor = 2.25;                              // height of the avatar in meters

    // 9.8 m/s^2 = x m/f * 30f/s

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

        if (currentView == 1) {
            // SDL_ShowCursor(SDL_DISABLE);
        } else {
            SDL_ShowCursor(SDL_ENABLE);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // Window was closed
                    currentView = -1;
                    break;
                case SDL_KEYDOWN:
                    pressedKeys[event.key.keysym.sym] = true;
                    break;
                case SDL_KEYUP:
                    pressedKeys[event.key.keysym.sym] = false;
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
                            mainView.handleResize(surf);
                    }  else if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        // The window was exposed and should be repainted.
                        redraw = true;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (currentView != 0) {
                                double deltaX = event.motion.xrel;
                                double deltaY = event.motion.yrel;
                                pitchDeg += (deltaY) * pixelsToDegrees;
                                yawDeg += (deltaX) * pixelsToDegrees;

                    }
                    redraw = true;
                    break;
            }
        } // End of event processing.

        // Keys are stored here to deal with the problem where there is a pause after an initial keypress to the repeating key presses.
        // For more info: https://gamedev.stackexchange.com/questions/63979/brief-pause-after-keypress
        if (pressedKeys[SDLK_ESCAPE]) {
            if (currentView == 1) {
                // Break out of main view.
                currentView = 0;
                redraw = true;
            } else if (currentView == 0) {
                // Break out of menu view (exit the program.)
                currentView = -1;
            }
        }

        if (pressedKeys[SDLK_w] || pressedKeys[SDLK_UP]) {
            if (currentView == 1) {
                velocity += normalize(mainView.getCamera().axisZ) * accelerationRate;
                if (velocity.magnitude() > maxVelocity) {
                    velocity = normalize(velocity) * maxVelocity;
                }
                redraw = true;
            }
        }

        if (pressedKeys[SDLK_s] || pressedKeys[SDLK_DOWN]) {
            if (currentView == 1) {
                velocity -= normalize(mainView.getCamera().axisZ) * accelerationRate;
                if (velocity.magnitude() > maxVelocity) {
                    velocity = normalize(velocity) * -maxVelocity;
                }
                redraw = true;
            }
        }

        if (pressedKeys[SDLK_a]) {
            if (currentView == 1) {
                velocity -= normalize(mainView.getCamera().axisX) * accelerationRate;
                if (velocity.magnitude() > maxVelocity) {
                    velocity = normalize(velocity) * -maxVelocity;
                }
                redraw = true;
            }
        }

        if (pressedKeys[SDLK_d]) {
            if (currentView == 1) {
                velocity += normalize(mainView.getCamera().axisX) * accelerationRate;
                if (velocity.magnitude() > maxVelocity) {
                    velocity = normalize(velocity) * maxVelocity;
                }
                redraw = true;
            }
        }

        if (pressedKeys[SDLK_SPACE]) {
            if (currentView == 1) {
                const double boosterPower = 1.50;
                const double maxVerticalSpeed = 5 / framesPerSecond; // units per Second
                const Vector up = normalize(mainView.getGrid().system().axisY);
                bool boostingAllowed = true;

                if (verticalMotion.magnitude() > maxVerticalSpeed) {
                    double cosTheta = dotProduct(normalize(verticalMotion), up);
                    if (cosTheta >= 0 && cosTheta < 1) {
                        // Going too fast, booster is not gonna turn on.
                        boostingAllowed = false;
                    }
                }
                if (boostingAllowed) {
                    verticalMotion += up * gravitationalAcceleration * boosterPower;
                }
                redraw = true;
            }
        }

        if (pressedKeys[SDLK_LEFT]) {
            if (currentView == 1) {
                currentTurningRate = std::min(currentTurningRate + angularAccelerationRate, maxTurningRate);
            }
        }

        if (pressedKeys[SDLK_RIGHT]) {
            if (currentView == 1) {
                currentTurningRate = std::max(currentTurningRate - angularAccelerationRate, -maxTurningRate);
            }
        }

        // Handle camera movement
        // For Euler rotation to work, the Euler angles (yaw, pitch, and roll)
        // have to take primacy.  That means that our camera basis must be
        // derived from them instead of being maintained separately.
        //
        // But we do need to at least move to the spot where the old camera
        // was, even if we don't care about its orientation.

        // Euler angles are only useful if they are done relative to the absolute frame of reference.
        Basis camera = {};
        Matrix actualCameraLocation = translationMatrix(Vector(mainView.getCamera().center));
        Matrix absoluteOrientation = eulerRotationMatrix(camera, yawDeg, pitchDeg, rollDeg);
        camera.apply(actualCameraLocation * absoluteOrientation); // Rotate first, then move to final spot.

        // Make sure that we move along the ground, even when our movement vector is
        // facing away from the ground, so we don't fly vertically when we are just walking.
        Plane gridPlane = mainView.getGrid().gridPlane();
        Vector gridVector = gridPlane.projection(velocity);
        if (gridVector.magnitude() > epsilon) {
            // Movements should be parallel to the grid plane, even if we are tilted.
            velocity = velocity.magnitude() * normalize(gridVector);
        }
        Vector momentaryVelocity = detectCollision(camera, velocity, mainView.getGrid(), heightFromFloor, gravitationalAcceleration);
        velocity = momentaryVelocity;
        camera.apply(translationMatrix(momentaryVelocity));

        // Vertical motion is handled seperatly from lateral motion
        verticalMotion = fallingVector(camera, verticalMotion, mainView.getGrid(), heightFromFloor, gravitationalAcceleration);
        camera.apply(translationMatrix(verticalMotion));

        // // DANGER WILL ROBINSON: GIMBAL LOCK
        // // This prevents gimbal lock by stopping you from tilting to 180 or 0 degrees like to the Grid's z axis
        // double currentElevation = calculateAbsoluteElevation(mainView.getGrid().system().axisY, camera.axisZ);
        // const double maxDeviationFromHorizon = 10;
        // if (currentElevation + thetaTilt >= 90 + maxDeviationFromHorizon) {
        //     thetaTilt = 90 + maxDeviationFromHorizon - currentElevation;
        // } else if (currentElevation + thetaTilt <= 90 - maxDeviationFromHorizon) {
        //     thetaTilt = 90 - maxDeviationFromHorizon - currentElevation;
        // }

        // Rotating according to the left and right arrow keys.
        yawDeg -= currentTurningRate;

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
                Renderer r;
                r.prepare(surf, mainView.getRenderBoundary(), camera);
                mainView.drawWithRenderer(r);

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
