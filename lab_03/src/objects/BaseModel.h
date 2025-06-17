#pragma once
#include "VisibleObject.h"


class BaseModel : public VisibleObject {
public:
    BaseModel();
    virtual ~BaseModel() = 0;

    virtual void Transform(const TransformAction& action) { (void) action;};
    virtual void accept(const Visitor &visitor) { (void) visitor; };
    virtual std::shared_ptr<Object> Clone() const { return nullptr;};
};
