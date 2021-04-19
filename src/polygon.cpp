#include "polygon.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "common.h"  // operator<<(ostream&, const SDL_Color&)

using namespace std;


Polygon::Polygon() : vertices() {
}

void Polygon::flip() {
    reverse(vertices.begin(), vertices.end());
}

Polygon triangle(Point a, Point b, Point c, SDL_Color x, SDL_Color y, SDL_Color z)  {
    std::vector<Vertex> vertices;
    // For now, these are copies rather than references, but we could change
    // that to reduce memory usage.
    vertices.push_back({ a, x, true });
    vertices.push_back({ b, y, true });
    vertices.push_back({ c, z, true });
    return Polygon(vertices, { 0, 1, 2 });
}

Polygon triangle(Point a, Point b, Point c, SDL_Color color) {
    return triangle(a, b, c, color, color, color);
}

Polygon triangle(Point a, Point b, Point c) {
    return triangle(a, b, c, SDL_Color{255, 255, 255, 255});
}

std::ostream& operator<<(std::ostream& out, const Polygon& poly) {
    using namespace std;

    ostringstream stream;
    stream.precision(2);
    stream << fixed;

    vector<string> pointStrings;
    size_t width = 0;
    for (const Vertex& v : poly.vertices) {
        const Point& p = v;
        stream.str("");
        stream << p;
        const string& s = stream.str();
        pointStrings.push_back(s);
        width = max(width, s.length());
    }

    for (size_t i = 0; i < poly.vertices.size(); ++i) {
        out << (i == 0 ? "Polygon: " : "         ");
        out << static_cast<char>('a' + i) << ": ";
        out << setw(width) << pointStrings[i] << " | ";
        out << static_cast<char>('A' + i) << static_cast<char>('A' + (i == poly.vertices.size() - 1 ? 0 : i + 1)) << ": " << (poly.vertices[i].visible ? "visible" : "hidden ") << " | ";

        stream.str("");
        stream << poly.vertices[i].color;
        out << stream.str() << "\n";
    }

    return out;
}
