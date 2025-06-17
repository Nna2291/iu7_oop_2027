#include "DrawVisitor.h"
#include "BoneModel.h"
#include "Camera.h"
#include "Composite.h"
#include "CameraProjectionAction.h"
#include "FaceBuilder.h"

#define FOCUS 500.0
#define R (1 / FOCUS)

DrawVisitor::DrawVisitor(std::shared_ptr<BaseDrawer> drawer, std::shared_ptr<Camera> camera) : _drawer(drawer), _camera(camera) {}

Point DrawVisitor::getCameraProjection(const Point& point) const {
    CameraProjectionAction action(_camera);
    Point copy(point);
    action.TransformPoint(copy);
    return copy;
}

Vector3 DrawVisitor::getCameraPosition() const {
    return Vector3(_camera->_self);
}

Vector3 DrawVisitor::calculateFaceNormal(const Point& p1, const Point& p2, const Point& p3) const {
    Vector3 v1(p1);
    Vector3 v2(p2);
    Vector3 v3(p3);
    
    Vector3 edge1 = v2 - v1;
    Vector3 edge2 = v3 - v1;
    
    return edge1.Cross(edge2).Normalize();
}

Point DrawVisitor::calculateModelCenter(const std::vector<Point>& points) const {
    if (points.empty()) return Point();
    
    double centerX = 0, centerY = 0, centerZ = 0;
    for (const auto& point : points) {
        centerX += point.GetX();
        centerY += point.GetY();
        centerZ += point.GetZ();
    }
    
    size_t count = points.size();
    return Point(centerX / count, centerY / count, centerZ / count);
}

bool DrawVisitor::isFaceVisible(const std::vector<Point>& points, const Face& face, const Vector3& cameraPos) const {
    if (face.size() < 3) return false;
    
    auto it = face.begin();
    int v1 = *it; ++it;
    int v2 = *it; ++it;
    int v3 = *it;
    
    if (static_cast<size_t>(v1) >= points.size() || static_cast<size_t>(v2) >= points.size() || static_cast<size_t>(v3) >= points.size()) {
        return false;
    }
    
    Point p1(points[v1]);
    Point p2(points[v2]);
    Point p3(points[v3]);
    
    Vector3 normal = calculateFaceNormal(p1, p2, p3);
    Vector3 faceCenter = Vector3((p1.GetX() + p2.GetX() + p3.GetX()) / 3.0,
                                 (p1.GetY() + p2.GetY() + p3.GetY()) / 3.0,
                                 (p1.GetZ() + p2.GetZ() + p3.GetZ()) / 3.0);
    
    Vector3 modelCenter = Vector3(calculateModelCenter(points));
    Vector3 toFace = faceCenter - modelCenter;
    
    if (normal.Dot(toFace) < 0) {
        normal = normal * -1;
    }
    
    Vector3 cameraVec = cameraPos - faceCenter;
    return normal.Dot(cameraVec) > 0;
}

void DrawVisitor::visit(BoneModel& model) const {
    auto points = model._model->GetPoints();
    auto edges = model._model->GetEdges();
    auto faces = model._model->GetFaces();
    
    if (faces.empty() && !edges.empty()) {
        faces = FaceBuilder::BuildFacesFromEdges(points, edges);
    }
    
    Vector3 cameraPos = getCameraPosition();
    
    if (_faceCullingEnabled && !faces.empty()) {
        for (const auto& face : faces) {
            if (isFaceVisible(points, face, cameraPos)) {
                for (auto it1 = face.begin(); it1 != face.end(); ++it1) {
                    for (auto it2 = face.begin(); it2 != face.end(); ++it2) {
                        if (it1 == it2) continue;
                        
                        bool isEdge = false;
                        for (const auto& edge : edges) {
                            if ((edge.GetFirst() == static_cast<size_t>(*it1) && edge.GetSecond() == static_cast<size_t>(*it2)) ||
                                (edge.GetFirst() == static_cast<size_t>(*it2) && edge.GetSecond() == static_cast<size_t>(*it1))) {
                                isEdge = true;
                                break;
                            }
                        }
                        
                        if (isEdge) {
                            Point p1(points[*it1]);
                            Point p2(points[*it2]);
                            Point proj1 = getCameraProjection(p1);
                            Point proj2 = getCameraProjection(p2);

                            proj1.SetX(proj1.GetX() * (1 / (R * proj1.GetZ())));
                            proj1.SetY(proj1.GetY() * (1 / (R * proj1.GetZ())));

                            proj2.SetX(proj2.GetX() * (1 / (R * proj2.GetZ())));
                            proj2.SetY(proj2.GetY() * (1 / (R * proj2.GetZ())));

                            if (proj1.GetZ() > 0 && proj2.GetZ() > 0) {
                                _drawer->DrawLine(proj1, proj2);
                            }
                        }
                    }
                }
            }
        }
    } else {
        for (auto &edge : edges) {
            Point &p1 = points[edge.GetFirst()];
            Point &p2 = points[edge.GetSecond()];
            Point proj1(getCameraProjection(p1));
            Point proj2(getCameraProjection(p2));

            proj1.SetX(proj1.GetX() * (1 / (R * proj1.GetZ())));
            proj1.SetY(proj1.GetY() * (1 / (R * proj1.GetZ())));

            proj2.SetX(proj2.GetX() * (1 / (R * proj2.GetZ())));
            proj2.SetY(proj2.GetY() * (1 / (R * proj2.GetZ())));

            if (proj1.GetZ() > 0 && proj2.GetZ() > 0) {
                _drawer->DrawLine(proj1, proj2);
            }
        }
    }
}

void DrawVisitor::visit(Camera& camera) const {
    (void) camera;
}
