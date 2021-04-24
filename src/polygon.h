#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include <initializer_list>
#include <vector>
#include <optional>
#include "point.h"
#include "SDL.h"

struct Vertex : public Point {
    public:
        SDL_Color color = {0, 255, 0, 255}; // Green by default.
};

// This class represents a 3-D polygon. Ultimately, this class is going to represent
// the triangles that we will subdivide the grid into.
struct Polygon {
    public:
        std::vector<Vertex> vertices;
    public:
        Polygon();

        template <typename V>
        Polygon(const std::vector<V>& vertexBuffer, std::initializer_list<int> indices) {
            for (auto iter = indices.begin(); iter != indices.end(); iter++) {
                int current_index = *iter;
                Vertex current_vertex;
                static_cast<Point&>(current_vertex) = static_cast<Point&>(vertexBuffer[current_index]);
                vertices.push_back(current_vertex);
            }
        }

        friend Polygon triangle(Point a, Point b, Point c, SDL_Color x, SDL_Color y, SDL_Color z);

        // For backface calling. Flips the polygon's normal vector that is facing the wrong way.
        void flip();

        // Remove parts of this polygon that are not on the same side of the clipPlane as its normal.
        std::optional<Polygon> clip(Plane clipPlane) const;

        friend std::ostream& operator<<(std::ostream&, Polygon polygon);
};

#endif // POLYGON_H_INCLUDED
