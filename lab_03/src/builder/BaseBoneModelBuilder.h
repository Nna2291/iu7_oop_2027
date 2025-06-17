#pragma once
#include "BoneModel.h"
#include "ModelStructure.h"
#include "BoneModelReader.h"
#include "CenterStrategy.h"

class BaseBoneModelBuilder {
public:
    BaseBoneModelBuilder() = default;

    virtual ~BaseBoneModelBuilder() = 0;

    virtual std::shared_ptr<BoneModel> Get();
    virtual void BuildPoint(const Point &p);
    virtual void BuildEdge(const Edge &edge);
    virtual void BuildCenter(const Point &center);
    virtual bool IsBuilt();

    virtual void BuildFromReader(std::shared_ptr<BoneModelReader> reader,
                                 std::shared_ptr<BaseCenterStrategy> strategy);

protected:
    virtual std::shared_ptr<ModelStructure> CreateModelStructure() = 0;
    std::shared_ptr<ModelStructure> _model;
};
