#include "GetObjectIDsSceneCommand.h"

GetObjectIDsSceneCommand::GetObjectIDsSceneCommand() : _ids(std::vector<std::size_t>()) {}

void GetObjectIDsSceneCommand::Execute() {
    _ids = _sceneManager->GetObjectIds();
}

std::string GetObjectIDsSceneCommand::GetDescription() const {
    return "Получение списка объектов";
}

std::vector<std::size_t> GetObjectIDsSceneCommand::GetIDs() {
    return _ids;
}