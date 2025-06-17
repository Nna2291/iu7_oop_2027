#include "RemoveObjectCommand.h"

RemoveObjectCommand::RemoveObjectCommand(std::size_t id) : _id(id) {}

void RemoveObjectCommand::Execute() {
    _sceneManager->RemoveObject(_id);
}

std::string RemoveObjectCommand::GetDescription() const {
    return "Удаление объекта " + std::to_string(_id);
}