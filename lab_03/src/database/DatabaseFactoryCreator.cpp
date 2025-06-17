#include "DatabaseFactoryCreator.h"
#include "SqliteFactory.h"
#include "PostgresqlFactory.h"

std::unique_ptr<AbstractDatabaseFactory> DatabaseFactoryCreator::CreateFactory(DatabaseType type) {
    switch (type) {
        case DB_SQLITE:
            return std::make_unique<SqliteFactory>();
        case DB_POSTGRESQL:
            return std::make_unique<PostgresqlFactory>();
        default:
            return nullptr;
    }
} 