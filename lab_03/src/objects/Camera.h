#pragma once
#include "InvisibleObject.h"
#include "TransformAction.h"
#include "DrawVisitor.h"
#include "CameraProjectionAction.h"

class Camera : public InvisibleObject {
    friend class DrawVisitor;
    friend class CameraProjectionAction;
public:
    Camera();
    Camera(double x, double y, double z);
    explicit Camera(const Point &p);
    explicit Camera(const Camera &other);

    virtual void Transform(const TransformAction &action) override;
    virtual void accept(const Visitor &visitor) override;
    virtual Point GetCenter() const override;
    virtual std::shared_ptr<Object> Clone() const override {
        return std::make_shared<Camera>(*this);
    }

    virtual ~Camera() = default;

protected:
    Point _self;
    Point _normalForward;
    Point _normalUp;
    Point _normalRight;
};
