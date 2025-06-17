#include "RotateObjectCommand.h"

RotateObjectCommand::RotateObjectCommand(std::size_t id, double ox, double oy, double oz)
: _id(id), _ox(ox), _oy(oy), _oz(oz) {}

void RotateObjectCommand::Execute() {
    auto obj = _sceneManager->GetObject(_id);
    _transformManager->RotateObject(obj, _ox, _oy, _oz);
}

std::string RotateObjectCommand::GetDescription() const {
    return "Поворот объекта " + std::to_string(_id) + 
           " на углы (" + std::to_string(_ox) + "°, " + 
           std::to_string(_oy) + "°, " + std::to_string(_oz) + "°)";
}