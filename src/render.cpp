#include "render.h"

Renderer::Renderer() : canvas(nullptr), viewPortRect(SDL_Rect{0, 0, 0, 0}), camera(), cameraMatrix(),
                       screenRect(SDL_Rect{0, 0, 0, 0}), projectionMatrix(), pixels(nullptr) {}

void Renderer::prepare(SDL_Surface* canvas, SDL_Rect viewPortRect, Basis camera) {
    this->canvas = canvas;
    this->viewPortRect = viewPortRect;
    this->camera = camera;

    // Transform any object in world space to camera space.
    // "camera space" is world space but with the camera at the origin.
    cameraMatrix = cameraTransform(camera.axisX, camera.axisY, camera.axisZ, camera.center);

    // Screen rect is the rectangle in the camera space that represents what the camera currently sees.
    // Growing this rectangle zooms the camera out.
    screenRect = {
        -63,
        -63,
        125,
        125
    };

    projectionMatrix = ::projectionMatrix(focalDistance, screenRect, viewPortRect);

    pixels = static_cast<uint32_t*>(canvas->pixels);
}

SDL_Surface* Renderer::getScreen() const{
    return canvas;
}
