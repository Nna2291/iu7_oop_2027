#pragma once
#include "DatabaseConnection.h"
#include <libpq-fe.h>
#include <string>
#include <functional>

class PostgresqlConnection : public DatabaseConnection {
public:
    PostgresqlConnection();
    virtual ~PostgresqlConnection();
    
    void Open(const std::string& filename) override;
    void Close() override;
    bool IsOpen() const override;
    void Execute(const std::string& sql, DatabaseCallback callback = nullptr, void* data = nullptr) override;
    std::string GetLastError() const override;

private:
    struct ConnectionConfig {
        std::string host = "localhost";
        std::string port = "5432";
        std::string user = "postgres";
        std::string password = "";
        std::string database = "postgres";
    };

    ConnectionConfig ParseConfigFile(const std::string& filename);
    std::string BuildConnectionString(const ConnectionConfig& config);
    
    PGconn* _conn;
    std::string _lastError;
}; 