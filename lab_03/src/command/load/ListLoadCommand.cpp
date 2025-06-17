#include "ListLoadCommand.h"

ListLoadCommand::ListLoadCommand() {
    _reader_id = 1;
    _director_id = 1;
}

void ListLoadCommand::Execute() {
    
}

std::string ListLoadCommand::GetDescription() const {
    return "Загрузка модели (список)";
}