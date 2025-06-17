#ifndef DATABASEFACTORYCREATOR_H
#define DATABASEFACTORYCREATOR_H

#include <memory>
#include "AbstractDatabaseFactory.h"

enum DatabaseType {
    DB_SQLITE,
    DB_MYSQL,
    DB_POSTGRESQL
};

class DatabaseFactoryCreator {
public:
    static std::unique_ptr<AbstractDatabaseFactory> CreateFactory(DatabaseType type);
};

#endif 