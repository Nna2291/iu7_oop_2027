#pragma once
#include <string>
#include <vector>

typedef int (*DatabaseCallback)(void* data, int argc, char** argv, char** colNames);

class DatabaseConnection {
public:
    virtual ~DatabaseConnection() = default;
    
    virtual void Open(const std::string& filename) = 0;
    
    virtual void Close() = 0;
    
    virtual bool IsOpen() const = 0;
    
    virtual void Execute(const std::string& sql, DatabaseCallback callback = nullptr, void* data = nullptr) = 0;
    
    virtual std::string GetLastError() const = 0;
}; 