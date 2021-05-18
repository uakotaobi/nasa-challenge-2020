#include "render.h"

#include <algorithm>
#include <cmath>

using namespace std;

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

Basis Renderer::getCamera() const{
    return camera;
}

void Renderer::drawLine(double x1, double y1, double x2, double y2, SDL_Color color) const {
    double x = x1;
    double y = y1;
    double maximum = max(abs(x2 - x1), abs(y2 - y1));
    for (int i = 0; i < maximum; i++) {
        // set pixel at (floor(x), floor(y));
        // offset formula: width * y + x
        unsigned int offset = canvas->w * static_cast<unsigned int>(y) + static_cast<unsigned int>(x);
        pixels[offset] = SDL_MapRGBA(canvas->format, color.r, color.g, color.b, color.a);
        x += (x2 - x1) / maximum;
        y += (y2 - y1) / maximum;
    }
}
