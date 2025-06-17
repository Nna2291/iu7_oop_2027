#pragma once
#include "Object.h"


class VisibleObject : public Object {
public:
    VisibleObject();
    virtual ~VisibleObject() = 0;

    bool IsVisible() const override { return true; };
};
