#include "BaseHistoryCommand.h"
#include "SceneManager.h"
#include "BoneModel.h"
#include <memory>
#include <iostream>

CareTaker& BaseHistoryCommand::GetCareTaker() {
    static CareTaker careTaker;
    return careTaker;
}

SaveStateCommand::SaveStateCommand(size_t objectId) : _objectId(objectId) {}

void SaveStateCommand::Execute() {
    std::cout << "SaveStateCommand::Execute() for object ID: " << _objectId << std::endl;
    auto scene = _sceneManager->GetScene();
    
    for (auto it = scene->begin(); it != scene->end(); ++it) {
        if ((*it)->GetId() == _objectId) {
            std::cout << "Found object with ID: " << _objectId << std::endl;
            if (auto boneModel = std::dynamic_pointer_cast<BoneModel>(*it)) {
                std::cout << "Object is BoneModel, creating memento..." << std::endl;
                auto memento = boneModel->CreateMemento();
                if (memento) {
                    GetCareTaker().SaveState(_objectId, memento);
                    std::cout << "Memento saved successfully" << std::endl;
                } else {
                    std::cout << "Failed to create memento" << std::endl;
                }
            } else {
                std::cout << "Object is not BoneModel" << std::endl;
            }
            break;
        }
    }
}

std::string SaveStateCommand::GetDescription() const {
    return "Сохранение состояния объекта ID: " + std::to_string(_objectId);
}

RestoreStateCommand::RestoreStateCommand(size_t objectId) : _objectId(objectId) {}

void RestoreStateCommand::Execute() {
    std::cout << "RestoreStateCommand::Execute() for object ID: " << _objectId << std::endl;
    auto memento = GetCareTaker().GetLastState(_objectId);
    if (!memento) {
        std::cout << "No memento found for object ID: " << _objectId << std::endl;
        return;
    }
    
    std::cout << "Found memento, restoring state..." << std::endl;
    auto scene = _sceneManager->GetScene();
    
    for (auto it = scene->begin(); it != scene->end(); ++it) {
        if ((*it)->GetId() == _objectId) {
            std::cout << "Found object with ID: " << _objectId << std::endl;
            if (auto boneModel = std::dynamic_pointer_cast<BoneModel>(*it)) {
                std::cout << "Restoring BoneModel..." << std::endl;
                boneModel->RestoreFromMemento(memento);
                std::cout << "State restored successfully" << std::endl;
            } else {
                std::cout << "Object is not BoneModel" << std::endl;
            }
            break;
        }
    }
}

std::string RestoreStateCommand::GetDescription() const {
    return "Восстановление состояния объекта ID: " + std::to_string(_objectId);
}

ClearHistoryCommand::ClearHistoryCommand(size_t objectId) : _objectId(objectId) {}

void ClearHistoryCommand::Execute() {
    GetCareTaker().ClearHistory(_objectId);
}

std::string ClearHistoryCommand::GetDescription() const {
    return "Очистка истории объекта ID: " + std::to_string(_objectId);
} 