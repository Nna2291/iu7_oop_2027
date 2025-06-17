#pragma once
#include "BaseBoneModelBuilder.h"
#include "MatrixModelStructure.h"


class MatrixBoneModelBuilder : public BaseBoneModelBuilder {
public:
    MatrixBoneModelBuilder();
    virtual ~MatrixBoneModelBuilder() = default;
protected:
    std::shared_ptr<ModelStructure> CreateModelStructure() override {
        return std::make_shared<MatrixModelStructure>();
    }
};
