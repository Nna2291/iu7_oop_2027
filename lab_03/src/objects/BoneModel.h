#pragma once
#include "BaseModel.h"
#include "ModelStructure.h"
#include "TransformAction.h"
#include <memory>
#include "DrawVisitor.h"
#include "Memento.h"


class Visitor;

class BoneModel : public BaseModel {
    friend class DrawVisitor;
public:
    BoneModel();
    explicit BoneModel(std::shared_ptr<ModelStructure>);
    explicit BoneModel(const BoneModel& other);
    ~BoneModel() = default;

    virtual void accept(const Visitor& visitor);
    virtual void Transform(const TransformAction& action);
    virtual Point GetCenter() const;
    
    std::shared_ptr<Memento> CreateMemento() const;
    void RestoreFromMemento(std::shared_ptr<Memento> memento);

protected:
    std::shared_ptr<ModelStructure> _model;

};
