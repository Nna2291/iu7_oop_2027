#pragma once
#include "Point.h"
#include <cmath>

class Vector3 {
public:
    Vector3() : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector3(const Point& p) : x(p.GetX()), y(p.GetY()), z(p.GetZ()) {}

    double GetX() const { return x; }
    double GetY() const { return y; }
    double GetZ() const { return z; }

    void SetX(double x) { this->x = x; }
    void SetY(double y) { this->y = y; }
    void SetZ(double z) { this->z = z; }

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(double scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(double scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    double Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    double Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalize() const {
        double len = Length();
        if (len > 0) {
            return *this / len;
        }
        return Vector3();
    }

    Point ToPoint() const {
        return Point(x, y, z);
    }

private:
    double x, y, z;
}; 