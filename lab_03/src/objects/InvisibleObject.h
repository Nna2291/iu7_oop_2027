#pragma once
#include "Object.h"


class InvisibleObject : public Object {
public:
    InvisibleObject();
    virtual ~InvisibleObject() = 0;

    bool IsVisible() const override { return false; };
};
