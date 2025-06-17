#include "MatrixLoadCommand.h"

MatrixLoadCommand::MatrixLoadCommand() {
    _reader_id = 1;
    _director_id = 2;
}

void MatrixLoadCommand::Execute() {
    _director_id = 1;
}

std::string MatrixLoadCommand::GetDescription() const {
    return "Загрузка модели с матричным представлением";
}