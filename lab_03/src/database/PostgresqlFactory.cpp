#include "PostgresqlFactory.h"
#include "PostgresqlConnection.h"

std::unique_ptr<DatabaseConnection> PostgresqlFactory::create() {
    return std::make_unique<PostgresqlConnection>();
} 