#include "ShiftObjectCommand.h"

ShiftObjectCommand::ShiftObjectCommand(std::size_t id, double x, double y, double z)
: _id(id), _x(x), _y(y), _z(z) {}

void ShiftObjectCommand::Execute() {
    auto obj = _sceneManager->GetObject(_id);
    _transformManager->ShiftObject(obj, _x, _y, _z);
}

std::string ShiftObjectCommand::GetDescription() const {
    return "Перемещение объекта " + std::to_string(_id) + 
           " на (" + std::to_string(_x) + ", " + 
           std::to_string(_y) + ", " + std::to_string(_z) + ")";
}