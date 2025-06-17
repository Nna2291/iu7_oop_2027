#pragma once
#include "BaseCommand.h"
#include <memory>
#include <vector>
#include <string>

class CommandManager {
public:
    CommandManager();
    ~CommandManager() = default;

    void ExecuteCommand(std::shared_ptr<BaseCommand> command);

}; 