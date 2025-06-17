#include "BoneModel.h"
#include "Visitor.h"


BoneModel::BoneModel() : _model(nullptr) {}

BoneModel::BoneModel(std::shared_ptr<ModelStructure> model) : _model(model) {}

BoneModel::BoneModel(const BoneModel& other) : _model(other._model->Clone()) {}

void BoneModel::accept(const Visitor& visitor) {
    visitor.visit(*this);
}

void BoneModel::Transform(const TransformAction& action) {
    _model->Transform(action);
}

Point BoneModel::GetCenter() const {
    return _model->GetCenter();
}

std::shared_ptr<Memento> BoneModel::CreateMemento() const {
    auto points = _model->GetPoints();
    auto center = _model->GetCenter();
    return std::make_shared<Memento>(points, center);
}

void BoneModel::RestoreFromMemento(std::shared_ptr<Memento> memento) {
    if (!memento) return;
    
    _model->ClearPoints();
    for (const auto& point : memento->GetPoints()) {
        _model->AddPoint(point);
    }
    
    _model->SetCenter(memento->GetCenter());
}
