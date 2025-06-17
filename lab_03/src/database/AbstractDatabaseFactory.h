#ifndef ABSTRACTDATABASEFACTORY_H
#define ABSTRACTDATABASEFACTORY_H

#include <memory>
#include "DatabaseConnection.h"

class AbstractDatabaseFactory
{
public:
    AbstractDatabaseFactory() = default;
    virtual ~AbstractDatabaseFactory() = default;
    virtual std::unique_ptr<DatabaseConnection> create() = 0;
};

#endif 