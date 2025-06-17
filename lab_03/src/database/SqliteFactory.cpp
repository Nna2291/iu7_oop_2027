#include "SqliteFactory.h"
#include "SqliteConnection.h"

std::unique_ptr<DatabaseConnection> SqliteFactory::create() {
    return std::make_unique<SqliteConnection>();
} 