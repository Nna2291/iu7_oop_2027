#include "ToggleFaceCullingCommand.h"
#include "DrawVisitor.h"

bool ToggleFaceCullingCommand::_faceCullingEnabled = true;

void ToggleFaceCullingCommand::Execute() {
    _faceCullingEnabled = !_faceCullingEnabled;
}

std::string ToggleFaceCullingCommand::GetDescription() const {
    return _faceCullingEnabled ? "Включить сокрытие граней" : "Отключить сокрытие граней";
} 