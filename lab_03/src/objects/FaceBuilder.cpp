#include "FaceBuilder.h"
#include <cmath>
#include <algorithm>

std::vector<Face> FaceBuilder::BuildFacesFromEdges(const std::vector<Point>& vertices, 
                                                   const std::vector<Edge>& edges) {
    std::vector<Face> faces;
    
    std::map<int, std::set<int>> adjacencyList;
    for (const auto& edge : edges) {
        int u = edge.GetFirst();
        int v = edge.GetSecond();
        adjacencyList[u].insert(v);
        adjacencyList[v].insert(u);
    }
    
    int count = 0;
    for (const auto& v1 : vertices) {
        for (const auto& [v2, neighbours] : adjacencyList) {
            if (neighbours.size() < 2) {
                continue;
            }

            Point vertex1(vertices[v2]);

            auto it1 = neighbours.begin();
            for (; it1 != neighbours.end(); ++it1) {
                auto it2 = std::next(it1);
                for (; it2 != neighbours.end(); ++it2) {
                    int element_1 = *it1;
                    int element_2 = *it2;

                    Point vertex2(vertices[element_1]);
                    Point vertex3(vertices[element_2]);

                    if (IsPointOnPlane(v1, vertex1, vertex2, vertex3)) {
                        bool added = false;
                        for (auto& surface : faces) {
                            if (surface.count(element_1) > 0 && surface.count(element_2) > 0 && surface.count(v2) > 0) {
                                surface.insert(count);
                                added = true;
                                break;
                            }
                        }

                        if (!added) {
                            Face newSurface = {element_1, element_2, v2, count};
                            faces.push_back(newSurface);
                        }
                    }
                }
            }
        }
        count++;
    }

    RemoveDuplicateFaces(faces);
    return faces;
}

bool FaceBuilder::IsPointOnPlane(const Point& p, const Point& p1, const Point& p2, const Point& p3) {
    Point v4 = Point(p.GetX() - p1.GetX(), p.GetY() - p1.GetY(), p.GetZ() - p1.GetZ());
    
    Point v2_v1 = Point(p2.GetX() - p1.GetX(), p2.GetY() - p1.GetY(), p2.GetZ() - p1.GetZ());
    Point v3_v1 = Point(p3.GetX() - p1.GetX(), p3.GetY() - p1.GetY(), p3.GetZ() - p1.GetZ());

    double x1 = v2_v1.GetY() * v3_v1.GetZ() - v2_v1.GetZ() * v3_v1.GetY();
    double y1 = v2_v1.GetZ() * v3_v1.GetX() - v2_v1.GetX() * v3_v1.GetZ();
    double z1 = v2_v1.GetX() * v3_v1.GetY() - v2_v1.GetY() * v3_v1.GetX();

    double dotProduct = x1 * v4.GetX() + y1 * v4.GetY() + z1 * v4.GetZ();

    return std::abs(dotProduct) < 1e-9;
}

void FaceBuilder::RemoveDuplicateFaces(std::vector<Face>& faces) {
    std::sort(faces.begin(), faces.end());
    faces.erase(std::unique(faces.begin(), faces.end()), faces.end());
} 