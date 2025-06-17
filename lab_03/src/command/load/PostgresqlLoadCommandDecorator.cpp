#include "PostgresqlLoadCommandDecorator.h"

PostgresqlLoadCommandDecorator::PostgresqlLoadCommandDecorator(BaseLoadCommand &command, const char *filename)
: BaseLoadCommandDecorator(command), _filename(filename) {
    _command = command;
    _reader_id = POSTGRESQLBONEMODELREADERCREATOR_ID;
}
void PostgresqlLoadCommandDecorator::Execute() {
    _command.Execute();
    _director_id = _command.GetDirectorId();
    auto obj = _loadManager->LoadBoneModelFile(_director_id, _reader_id, _filename);
    _sceneManager->AddObject(obj);
}

std::string PostgresqlLoadCommandDecorator::GetDescription() const {
    return "Загрузка модели из PostgreSQL базы данных: " + std::string(_filename);
} 