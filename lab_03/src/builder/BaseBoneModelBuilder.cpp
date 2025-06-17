#include "BaseBoneModelBuilder.h"

BaseBoneModelBuilder::~BaseBoneModelBuilder() {}

void BaseBoneModelBuilder::BuildPoint(const Point& p) {
    _model->AddPoint(p);
}

void BaseBoneModelBuilder::BuildEdge(const Edge& edge) {
    _model->AddEdge(edge);
}

void BaseBoneModelBuilder::BuildCenter(const Point& center) {
    _model->SetCenter(center);
}

std::shared_ptr<BoneModel> BaseBoneModelBuilder::Get() {
    return std::make_shared<BoneModel>(_model);
}

bool BaseBoneModelBuilder::IsBuilt() {
    return _model->GetPoints().size() != 0;
}

void BaseBoneModelBuilder::BuildFromReader(std::shared_ptr<BoneModelReader> reader,
                                           std::shared_ptr<BaseCenterStrategy> strategy) {
    if (!reader) {
        throw std::runtime_error("Builder::BuildFromReader: reader is null");
    }

    if (!strategy) {
        throw std::runtime_error("Builder::BuildFromReader: strategy is null");
    }

    _model = CreateModelStructure();

    reader->Open();
    auto points = reader->ReadPoints();
    auto edges = reader->ReadEdges();
    reader->Close();

    for (const auto& p : points) {
        BuildPoint(p);
    }

    for (const auto& e : edges) {
        BuildEdge(e);
    }

    BuildCenter(strategy->CenterAlgorithm(points));
}
