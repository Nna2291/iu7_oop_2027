#include "Composite.h"
#include "Visitor.h"
#include "CenterStrategy.h"

Composite::Composite(std::vector<std::shared_ptr<Object>> vec) : _objects(vec) {}

bool Composite::Add(std::shared_ptr<Object> obj) {
    if (!obj) {
        return false;
    }
    _objects.push_back(std::move(obj));
    return true;
}

bool Composite::Remove(const iterator& it) {
    if (it == _objects.end()) {
        return false;
    }
    _objects.erase(it);
    return true;
}

std::shared_ptr<Object> Composite::Clone() const {
    std::shared_ptr<Composite> copy = std::make_shared<Composite>();
    for (auto &obj : _objects) {
        copy->Add(obj->Clone());
    }
    return copy;
}

void Composite::Transform(const TransformAction &action) {
    for (auto &obj : _objects) {
        obj->Transform(action);
    }
}

void Composite::accept(const Visitor &visitor) {
    for (auto &obj : _objects) {
        obj->accept(visitor);
    }
}

Composite::iterator Composite::begin() {
    return _objects.begin();
}

Composite::iterator Composite::end() {
    return _objects.end();
}

bool Composite::IsVisible() const { return true; }

Point Composite::GetCenter() const {
    std::vector<Point> centers;
    for (auto &obj : _objects) {
        centers.push_back(obj->GetCenter());
    }
    ClassicCenterStrategy centerStrategy;
    return centerStrategy.CenterAlgorithm(centers);
}
