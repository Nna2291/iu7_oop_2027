#pragma once
#include "DatabaseConnection.h"
#include <sqlite3.h>

class SqliteConnection : public DatabaseConnection {
public:
    SqliteConnection();
    virtual ~SqliteConnection();
    
    virtual void Open(const std::string& filename) override;
    virtual void Close() override;
    virtual bool IsOpen() const override;
    virtual void Execute(const std::string& sql, DatabaseCallback callback = nullptr, void* data = nullptr) override;
    virtual std::string GetLastError() const override;

private:
    sqlite3* _db;
    std::string _lastError;
}; 