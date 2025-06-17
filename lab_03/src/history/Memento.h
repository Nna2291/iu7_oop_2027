#pragma once
#include "Point.h"
#include <vector>
#include <memory>

class Memento {
public:
    Memento() = delete;
    explicit Memento(const std::vector<Point>& points, const Point& center);
    ~Memento() = default;

    const std::vector<Point>& GetPoints() const { return _points; }
    const Point& GetCenter() const { return _center; }

private:
    std::vector<Point> _points;
    Point _center;
}; 