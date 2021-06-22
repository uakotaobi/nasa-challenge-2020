#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "SDL.h"
#include "basis.h"
#include "matrix.h"
#include "polygon.h"

#include <iostream>

class Renderer {
    public:
        Renderer();

        // This function is called at the beginning of the frame that you are rendering.
        void prepare(SDL_Surface* canvas, SDL_Rect viewPortRect, Basis camera);

        // This exposes our internal SDL_Surface so that other people can use it.
        SDL_Surface* getScreen() const;

        // The camera is not exposed in the render so we can get it here.
        Basis getCamera() const;

        template <typename ColorPointIterator>
        void renderPoint(ColorPointIterator begin, ColorPointIterator end) const {
            for (ColorPointIterator iter = begin; iter != end; ++iter) {
                Point p = static_cast<Point>(*iter); // Making an explicit copy here
                SDL_Color color = iter->color;

                p = cameraMatrix * p;
                if (p.z < 0) {
                    continue;
                }
                p = projectionMatrix * p;

                // Any point out of bounds of the view rectangle is skipped.
                if (p.x < viewPortRect.x ||
                    p.y < viewPortRect.y ||
                    p.x >= viewPortRect.x + viewPortRect.w ||
                    p.y >= viewPortRect.y + viewPortRect.h) {
                        continue;
                }

                // Render points that weren't skipped.
                // Offset formula:
                // width*y+x
                unsigned int offset = canvas->w * static_cast<unsigned int>(p.y) + static_cast<unsigned int>(p.x);
                pixels[offset] = SDL_MapRGBA(canvas->format, color.r, color.g, color.b, color.a);
            }
        }

        // Renders a set of polygons on the screen.
        template <typename PolygonIterator>
        void renderPolygon(PolygonIterator begin, PolygonIterator end) const {
            int counter = 0;

            Plane nearClipPlane = Plane(0, 0, 1, 0); // z = 0
            auto viewPortClipPlanes = {
                Plane(0, 1, 0, -viewPortRect.y),                  // y = viewPortRect.y (top)
                Plane(1, 0, 0, -viewPortRect.x),                  // x = viewPortRect.x (left)
                Plane(0, -1, 0, viewPortRect.y + viewPortRect.h), // -y = -viewPortRect.y - viewPortRect.h (bottom)
                Plane(-1, 0, 0, viewPortRect.x + viewPortRect.w), // -x = -viewPortRect.x - viewPortRect.w (right)
            };

            // For each polygon:
            for (PolygonIterator iter = begin; iter != end; ++iter) {
                Polygon poly = *iter;

                //   Convert every vertex from world space to camera space.
                for (Vertex& v : poly.vertices) {
                    (Point&)v = cameraMatrix * v;
                }

                //   Clip the polygon against the plane z = 0.
                auto clipPoly = poly.clip(nearClipPlane);

                //   If the polygon is clipped away, then skip.
                if (clipPoly == std::nullopt) {
                    continue;
                }

                //   Project the polygon into viewport space.
                for (Vertex& v : clipPoly->vertices) {
                    (Point&)v = projectionMatrix * v;
                }

                //   Clip the polygon against the four viewport planes (top, bottom, left, right).
                bool clippedAway = false;
                for (auto viewPortClipPlane : viewPortClipPlanes) {
                    auto viewPortClipPoly = poly.clip(viewPortClipPlane);

                    if (viewPortClipPoly == std::nullopt) {
                        clippedAway = true;
                        break;
                    }
                    poly = *viewPortClipPoly;
                }

                //   If the polygon is clipped away, then skip.
                if (clippedAway == true) {
                    continue;
                }
                //   For each vertex in the polygon:
                for (int i = 0; i < poly.vertices.size(); i++) {
                    //     Draw a line from each vertex to the next, using the current vertex's color.
                    Vertex currentVertex = poly.vertices[i];
                    Vertex nextVertex;
                    if (i != poly.vertices.size() - 1) {
                        nextVertex = poly.vertices[i + 1];
                    } else {
                        nextVertex = poly.vertices[0];
                    }
                    drawLine(currentVertex.x, currentVertex.y, nextVertex.x, nextVertex.y, currentVertex.color);
                }
                counter = counter + 1;
            } // End (for each polygon)
            // std::cout << counter << "\n";
        }

    private:
        const double focalDistance = 60;

        // These variables change from frame to frame.
        SDL_Surface* canvas;
        SDL_Rect viewPortRect;
        Basis camera;

        Matrix cameraMatrix;
        SDL_Rect screenRect;
        Matrix projectionMatrix;
        uint32_t* pixels;

        // Draws a line from (x1, y1) to (x2, y2) in the given color.
        void drawLine(double x1, double y1, double x2, double y2, SDL_Color color) const;
};

#endif // RENDER_H_INCLUDED
