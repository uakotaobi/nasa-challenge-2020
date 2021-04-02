#include "polygon.h"
#include <algorithm>

using namespace std;

Polygon::Polygon() : vertices() {
}

void Polygon::flip() {
    reverse(vertices.begin(), vertices.end());
}