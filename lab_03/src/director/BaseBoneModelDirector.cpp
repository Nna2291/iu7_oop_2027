#include "BaseBoneModelDirector.h"
#include "directorexception.h"


BaseBoneModelDirector::~BaseBoneModelDirector() {};

BaseBoneModelDirector::BaseBoneModelDirector(std::shared_ptr<BoneModelReader> reader)
    : _strategy(std::make_shared<ClassicCenterStrategy>()), _reader(reader)  {}

void BaseBoneModelDirector::Create() {
    if (!_reader) {
        throw std::runtime_error("BaseBoneModelDirector: Reader not set");
    }
    if (!_builder) {
        throw std::runtime_error("BaseBoneModelDirector: Builder not set");
    }

    _builder->BuildFromReader(_reader, _strategy);
}

std::shared_ptr<Object> BaseBoneModelDirector::Get() {
    return _builder->Get();
}
