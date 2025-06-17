#include "CommandManager.h"
#include <iostream>

CommandManager::CommandManager() {
}

void CommandManager::ExecuteCommand(std::shared_ptr<BaseCommand> command) {
    if (!command) {
        return;
    }
    
    try {
        command->Execute();
    } catch (const std::exception& e) {
        std::cerr << "Error executing command: " << e.what() << std::endl;
        throw;
    }
} 