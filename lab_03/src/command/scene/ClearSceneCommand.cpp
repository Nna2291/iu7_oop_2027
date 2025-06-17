#include "ClearSceneCommand.h"


void ClearSceneCommand::Execute() {
    _sceneManager->ClearScene();
}

std::string ClearSceneCommand::GetDescription() const {
    return "Очистка сцены";
}