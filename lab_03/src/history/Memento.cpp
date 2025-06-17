#include "Memento.h"

Memento::Memento(const std::vector<Point>& points, const Point& center) 
    : _points(points), _center(center) {
} 