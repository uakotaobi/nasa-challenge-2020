#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include <initializer_list>
#include <vector>
#include <optional>
#include <iostream>
#include "point.h"
#include "plane.h"
#include "SDL.h"

struct Vertex : public Point {
    public:
        SDL_Color color = {0, 255, 0, 255}; // Green by default.
        bool visible = true;                // True if the edge from this vertex to the next is visible.
};

struct Polygon {
    public:
        std::vector<Vertex> vertices;
    public:
        Polygon();

        // Creates a polygon given a list of vertices and an array of indices
        // providing connectivity information.
        template <typename V>
        Polygon(const std::vector<V>& vertexBuffer, std::initializer_list<int> indices) {
            for (auto iter = indices.begin(); iter != indices.end(); iter++) {
                int current_index = *iter;
                Vertex current_vertex;
                static_cast<Point&>(current_vertex) = static_cast<const Point&>(vertexBuffer[current_index]);
                vertices.push_back(current_vertex);
            }
        }

        // As above, but the vertex list is an initializer list (useful for
        // specifying smaller polygons a bit more easily.)
        template <typename V>
        Polygon(const std::initializer_list<V> vertexBuffer, std::initializer_list<int> indices) {
            for (auto iter = indices.begin(); iter != indices.end(); iter++) {
                int current_index = *iter;
                Vertex current_vertex;
                static_cast<Point&>(current_vertex) = static_cast<const Point&>(*(vertexBuffer.begin() + current_index));
                vertices.push_back(current_vertex);
            }
        }

        // Clips the polygon to the given plane, preserving only the part(s)
        // that are on the same side of the plane as the plane's normal.
        //
        // Returns the clipped polygon, which may have more vertices than
        // *this.  If this function returns nullopt, the entire polygon was
        // clipped away.
        //
        // NOTE:
        //     In cases where clipping a concave polygon would result in two
        //     or more separate polygons (think of a vertical slice through
        //     Pac-Man's mouth), this function will only return a single
        //     connected polygon.  This clipped polygon will contain all of
        //     the vertices that survived the clipping, and some portion of it
        //     will have a width of 0.
        //
        //     The correct behavior would be to return an array of polygons,
        //     but the current algorithm does not support that.
        std::optional<Polygon> clip(Plane P) const {
            Polygon result;

            // This is a brief implementation of the Sutherland-Hodgeman
            // clipping algorithm.
            for (auto iter = vertices.cbegin(); iter != vertices.cend(); ++iter) {

                const Vertex& p = *iter;
                const Vertex& q = (iter + 1 == vertices.cend() ? *vertices.cbegin() : *(iter + 1));

                const double ps = P.whichSide(p);
                const double qs = P.whichSide(q);

                // Given the plane and the points P and Q, we have just four
                // possibilities:
                if (ps >= 0 && qs >= 0) {        // Both points are inside (push P.)
                    result.vertices.push_back(p);
                    // std::cout << "  [In -> In]  " << (Point)p << " had visibility " << (p.visible ? "true" : "false") << "\n";
                } else if (ps >= 0 && qs < 0) { // P inside, Q outside (push P and intersection.)
                    Point clipPoint = *P.pointOfIntersection(p, q);
                    Vertex r { clipPoint, p.color, p.visible };
                    result.vertices.push_back(p);
                    // std::cout << "  [In -> Out] " << (Point)p << " had visibility " << (p.visible ? "true" : "false") << "\n";
                    result.vertices.push_back(r);
                    // std::cout << "  [In -> Out] " << (Point)r << " inherits visibility from " << p << "\n";
                } else if (ps < 0 && qs >= 0) { // P outside, Q inside (push intersection.)
                    Point clipPoint = *P.pointOfIntersection(p, q);
                    Vertex r { clipPoint, q.color, p.visible };
                    // Stupid special case: if the point prior to p was hidden
                    // and inside AND we're outside, then we need to be
                    // hidden, too.
                    if (iter != vertices.cbegin() && P.whichSide(*(iter - 1)) >= 0 && ps < 0 && !(iter - 1)->visible) {
                        r.visible = (iter - 1)->visible;
                        // std::cout << "  [Out -> In special] " << (Point)r << " inherits visibility from " << (Point)(*(iter - 1)) << "\n";
                    } else {
                        // std::cout << "  [Out -> In] " << (Point)r << " inherits visibility from " << p << "\n";
                    }
                    result.vertices.push_back(r);
                } else {                       // Both points are outside (do nothing.)
                    // std::cout << "  [Out -> Out]\n";
                }
            }
            return (result.vertices.size() > 0 ? std::optional<Polygon>(result) : std::nullopt);
        }

        friend Polygon triangle(Point a, Point b, Point c, SDL_Color x, SDL_Color y, SDL_Color z);
        friend Polygon triangle(Point a, Point b, Point c, SDL_Color x);
        friend Polygon triangle(Point a, Point b, Point c);
        friend std::ostream& operator<<(std::ostream& out, const Polygon& poly);

        // For backface calling. Flips the polygon's normal vector that is facing the wrong way.
        void flip();
};

#endif // POLYGON_H_INCLUDED
