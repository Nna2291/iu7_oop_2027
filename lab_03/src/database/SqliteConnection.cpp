#include "SqliteConnection.h"
#include "databaseexception.h"
#include <ctime>

SqliteConnection::SqliteConnection() : _db(nullptr) {}

SqliteConnection::~SqliteConnection() {
    if (IsOpen()) {
        Close();
    }
}

bool SqliteConnection::IsOpen() const {
    return _db != nullptr;
}

void SqliteConnection::Open(const std::string& filename) {
    if (IsOpen()) {
        Close();
    }
    
    int rc = sqlite3_open(filename.c_str(), &_db);
    if (rc != SQLITE_OK) {
        _lastError = "Cannot open database: " + std::string(sqlite3_errmsg(_db));
        sqlite3_close(_db);
        _db = nullptr;
        
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   _lastError.c_str());
    }
    
    _lastError.clear();
}

void SqliteConnection::Close() {
    if (_db) {
        sqlite3_close(_db);
        _db = nullptr;
    }
}

void SqliteConnection::Execute(const std::string& sql, DatabaseCallback callback, void* data) {
    if (!IsOpen()) {
        time_t now = time(nullptr);
        throw DatabaseConnectionException(ctime(&now), __FILE__, __LINE__, 
                                        typeid(*this).name(), __FUNCTION__, 
                                        "Database is not open");
    }
    
    char* errorMsg = nullptr;
    int rc = sqlite3_exec(_db, sql.c_str(), callback, data, &errorMsg);
    
    if (rc != SQLITE_OK) {
        _lastError = "SQL error: " + std::string(errorMsg ? errorMsg : "Unknown error");
        if (errorMsg) {
            sqlite3_free(errorMsg);
        }
        
        time_t now = time(nullptr);
        throw DatabaseQueryException(ctime(&now), __FILE__, __LINE__, 
                                    typeid(*this).name(), __FUNCTION__, 
                                    _lastError.c_str());
    }
    
    _lastError.clear();
}

std::string SqliteConnection::GetLastError() const {
    return _lastError;
}
