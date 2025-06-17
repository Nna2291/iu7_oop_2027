#include "CompositeObjectCommand.h"

CompositeObjectCommand::CompositeObjectCommand(std::vector<size_t> ids) : _ids(ids) {}

void CompositeObjectCommand::Execute() {
    _sceneManager->MakeComposite(_ids);
}

std::string CompositeObjectCommand::GetDescription() const {
    return "Создание композитного объекта";
}
