#include "polygon.h"
#include <algorithm>

using namespace std;

Polygon::Polygon() : vertices() {
}

void Polygon::flip() {
    reverse(vertices.begin(), vertices.end());
}

std::optional<Polygon> Polygon::clip(Plane clipPlane) {
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
        // TODO:
        // Sutherland-Hodgeman Algorithm
    }
        
};