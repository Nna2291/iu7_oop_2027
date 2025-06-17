#pragma once
#include "Visitor.h"
#include "BaseDrawer.h"
#include "Vector3.h"
#include "ModelStructure.h"

class DrawVisitor : public Visitor {
public:
    DrawVisitor() = delete;
    DrawVisitor(std::shared_ptr<BaseDrawer> drawer, std::shared_ptr<Camera> camera);
    ~DrawVisitor() = default;

    virtual void visit(BoneModel& model) const;
    virtual void visit(Camera& camera) const;

    void SetFaceCullingEnabled(bool enabled) { _faceCullingEnabled = enabled; }
    bool IsFaceCullingEnabled() const { return _faceCullingEnabled; }

private:
    Point getCameraProjection(const Point& point) const;
    bool isFaceVisible(const std::vector<Point>& points, const Face& face, const Vector3& cameraPos) const;
    Vector3 getCameraPosition() const;
    Vector3 calculateFaceNormal(const Point& p1, const Point& p2, const Point& p3) const;
    Point calculateModelCenter(const std::vector<Point>& points) const;
    
    std::shared_ptr<BaseDrawer> _drawer;
    std::shared_ptr<Camera> _camera;
    bool _faceCullingEnabled = true;
};
