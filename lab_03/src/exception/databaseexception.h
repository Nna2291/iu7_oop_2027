#pragma once
#include "baseexception.h"

class DatabaseException : public BaseException {
public:
    DatabaseException(const char* time, const char* filename, 
                     size_t line, const char* classname, const char* method,
                     const char* info = "Database error") 
        : BaseException(time, filename, line, classname, method, info) {}
    
    virtual ~DatabaseException() = default;
};

class DatabaseConnectionException : public DatabaseException {
public:
    DatabaseConnectionException(const char* time, const char* filename, 
                               size_t line, const char* classname, const char* method,
                               const char* info = "Database connection error") 
        : DatabaseException(time, filename, line, classname, method, info) {}
    
    virtual ~DatabaseConnectionException() = default;
};

class DatabaseQueryException : public DatabaseException {
public:
    DatabaseQueryException(const char* time, const char* filename, 
                          size_t line, const char* classname, const char* method,
                          const char* info = "Database query error") 
        : DatabaseException(time, filename, line, classname, method, info) {}
    
    virtual ~DatabaseQueryException() = default;
};

class DatabaseOpenException : public DatabaseConnectionException {
public:
    DatabaseOpenException(const char* time, const char* filename, 
                         size_t line, const char* classname, const char* method,
                         const char* info = "Cannot open database") 
        : DatabaseConnectionException(time, filename, line, classname, method, info) {}
    
    virtual ~DatabaseOpenException() = default;
};

class DatabaseCloseException : public DatabaseConnectionException {
public:
    DatabaseCloseException(const char* time, const char* filename, 
                          size_t line, const char* classname, const char* method,
                          const char* info = "Cannot close database") 
        : DatabaseConnectionException(time, filename, line, classname, method, info) {}
    
    virtual ~DatabaseCloseException() = default;
};

class DatabaseTypeException : public DatabaseException {
public:
    DatabaseTypeException(const char* time, const char* filename, 
                         size_t line, const char* classname, const char* method,
                         const char* info = "Unknown database type") 
        : DatabaseException(time, filename, line, classname, method, info) {}
    
    virtual ~DatabaseTypeException() = default;
}; 