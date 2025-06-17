#pragma once
#include "TransformAction.h"
#include <memory>
#include <vector>
#include <cstddef>

class Visitor;

std::size_t GetNextId();

class Object {
public:
    using iterator = std::vector<std::shared_ptr<Object>>::iterator;
    Object();
    virtual ~Object() = 0;

    virtual bool Add(std::shared_ptr<Object> obj) { (void)obj; return false; }
    virtual bool Remove(const iterator& it) { (void)it; return false; }

    virtual iterator begin() { return iterator(); };
    virtual iterator end() { return iterator(); };
    virtual bool IsComposite() { return false; }

    virtual void Transform(const TransformAction& action) = 0;
    virtual void accept(const Visitor &visitor) = 0;
    virtual std::shared_ptr<Object> Clone() const = 0;

    virtual std::size_t GetId() const { return _id; };

    virtual Point GetCenter() const = 0;
    virtual bool IsVisible() const = 0;

protected:
    std::size_t _id = 0;
};
