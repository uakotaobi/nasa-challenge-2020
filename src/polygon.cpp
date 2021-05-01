#include "polygon.h"
#include "common.h"
#include <algorithm>
#include <sstream>

using namespace std;

Polygon::Polygon() : vertices() {
}

void Polygon::flip() {
    reverse(vertices.begin(), vertices.end());
}

std::optional<Polygon> Polygon::clip(Plane clipPlane) const {
    Polygon clippedPolygon;
    for (auto iter = vertices.begin(); iter != vertices.end(); iter++) {
        Vertex p = *iter;
        Vertex q;
        if (iter + 1 == vertices.end()) {
            // Iter is on the final vertex.
            q = *vertices.begin();
        } else {
            // Iter is not on the final vertex.
            q = *(iter + 1);
        }

        // Test if there is an intersection between line segment pq and clipPlane.
        optional<Point> p_intersection = clipPlane.pointOfIntersection(p, q);
        Vertex intersection;
        if (p_intersection != nullopt) {
            intersection = {*p_intersection, p.color};
        }

        if (clipPlane.whichSide(p) > 0 && clipPlane.whichSide(q) > 0) {
            // Both p and q are on the inside.
            clippedPolygon.vertices.push_back(p);
        } else if (clipPlane.whichSide(p) > 0 && clipPlane.whichSide(q) <= 0) {
            // P is inside and q is outside.
            clippedPolygon.vertices.push_back(p);
            clippedPolygon.vertices.push_back(intersection);
        } else if (clipPlane.whichSide(p) <= 0 && clipPlane.whichSide(q) > 0) {
            // P is outside and q is inside.
            clippedPolygon.vertices.push_back(intersection);
        } else {
            // Both p and q are outside. Do nothing!
        }
    }

    if (clippedPolygon.vertices.size() == 0) {
        // The polygon was entirely on the outside of clipPlane.
        return nullopt;
    } else {
        return std::optional<Polygon> (clippedPolygon);
    }
};

// Prints a polygon to an output stream.
ostream& operator<<(ostream& out, const Polygon& polygon) {
    // Calculate length of longest point when it is printed
    int maxSize = 0;
    vector<string> pointStrings;
    for (auto vertex : polygon.vertices) {
        stringstream ss;
        ss << vertex;
        pointStrings.push_back(ss.str());
        if (ss.str().size() > maxSize) {
            maxSize = ss.str().size();
        }
    }

    for (int i = 0; i < polygon.vertices.size(); i++) {
        char letter = i + 'A';
        string prefix;
        if (i == 0) {
            prefix = "Polygon:\t";
        } else {
            prefix = "        \t";
        }
        out << prefix << letter << ":\t";
        out.width(maxSize);
        out << pointStrings[i] << " | " << "Color: "
            << polygon.vertices[i].color << "\n";
    }
    return out;
}
