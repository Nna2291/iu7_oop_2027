#pragma once
#include "Point.h"
#include "Edge.h"
#include "ModelStructure.h"
#include <vector>
#include <map>
#include <set>

class FaceBuilder {
public:
    FaceBuilder() = default;

    static std::vector<Face> BuildFacesFromEdges(const std::vector<Point>& vertices, 
                                                 const std::vector<Edge>& edges);

private:
    static bool IsPointOnPlane(const Point& p, const Point& p1, const Point& p2, const Point& p3);
    static void RemoveDuplicateFaces(std::vector<Face>& faces);
}; 