#pragma once
#include "BoneModel.h"
#include "BaseBoneModelBuilder.h"
#include "ListModelStructure.h"


class ListBoneModelBuilder : public BaseBoneModelBuilder {
public:
    ListBoneModelBuilder();

    virtual ~ListBoneModelBuilder() = default;
protected:
    std::shared_ptr<ModelStructure> CreateModelStructure() override {
        return std::make_shared<ListModelStructure>();
    }
};
