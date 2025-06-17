#include "PostgresqlConnection.h"
#include "databaseexception.h"
#include <fstream>
#include <sstream>
#include <ctime>

PostgresqlConnection::PostgresqlConnection() : _conn(nullptr) {}

PostgresqlConnection::~PostgresqlConnection() {
    if (IsOpen()) {
        Close();
    }
}

bool PostgresqlConnection::IsOpen() const {
    return _conn != nullptr && PQstatus(_conn) == CONNECTION_OK;
}

PostgresqlConnection::ConnectionConfig PostgresqlConnection::ParseConfigFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Cannot open config file");
    }
    
    std::string line;
    if (!std::getline(file, line)) {
        file.close();
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Config file is empty");
    }
    file.close();
    
    size_t end = line.find_last_not_of(" \t\r\n");
    if (end != std::string::npos) {
        line = line.substr(0, end + 1);
    }
    
    if (line.empty()) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Config file contains only whitespace");
    }
    
    ConnectionConfig config;
    
    size_t atPos = line.find('@');
    if (atPos == std::string::npos) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: missing '@' symbol. Expected format: user:password@host:port");
    }
    
    std::string userPass = line.substr(0, atPos);
    std::string hostPort = line.substr(atPos + 1);
    
    if (userPass.empty()) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: empty user:password part");
    }
    
    if (hostPort.empty()) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: empty host:port part");
    }
    
    size_t colonPos = userPass.find(':');
    if (colonPos == std::string::npos) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: missing ':' in user:password part");
    }
    
    config.user = userPass.substr(0, colonPos);
    config.password = userPass.substr(colonPos + 1);
    
    if (config.user.empty()) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: empty username");
    }
    
    if (config.password.empty()) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: empty password");
    }
    
    colonPos = hostPort.find(':');
    if (colonPos == std::string::npos) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: missing ':' in host:port part");
    }
    
    config.host = hostPort.substr(0, colonPos);
    config.port = hostPort.substr(colonPos + 1);
    
    if (config.host.empty()) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: empty hostname");
    }
    
    if (config.port.empty()) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: empty port");
    }
    
    try {
        int portNum = std::stoi(config.port);
        if (portNum <= 0 || portNum > 65535) {
            time_t now = time(nullptr);
            throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                       typeid(*this).name(), __FUNCTION__, 
                                       "Invalid config format: port must be between 1 and 65535");
        }
    } catch (const std::exception&) {
        time_t now = time(nullptr);
        throw DatabaseOpenException(ctime(&now), __FILE__, __LINE__, 
                                   typeid(*this).name(), __FUNCTION__, 
                                   "Invalid config format: port must be a valid number");
    }
    
    return config;
}

std::string PostgresqlConnection::BuildConnectionString(const ConnectionConfig& config) {
    std::ostringstream oss;
    oss << "host=" << config.host 
        << " port=" << config.port 
        << " user=" << config.user 
        << " password=" << config.password 
        << " dbname=" << config.database;
    return oss.str();
}

void PostgresqlConnection::Open(const std::string& filename) {
    if (IsOpen()) {
        Close();
    }
    
    try {
        ConnectionConfig config = ParseConfigFile(filename);
        std::string connStr = BuildConnectionString(config);
        
        connStr += " connect_timeout=2";
        
        _conn = PQconnectdb(connStr.c_str());
        
        if (PQstatus(_conn) != CONNECTION_OK) {
            _lastError = PQerrorMessage(_conn);
            PQfinish(_conn);
            _conn = nullptr;
            
            time_t now = time(nullptr);
            throw DatabaseConnectionException(ctime(&now), __FILE__, __LINE__, 
                                            typeid(*this).name(), __FUNCTION__, 
                                            _lastError.c_str());
        }
        
        PGresult* testRes = PQexec(_conn, "SELECT 1");
        if (PQresultStatus(testRes) != PGRES_TUPLES_OK) {
            _lastError = "Connection test failed: ";
            _lastError += PQerrorMessage(_conn);
            PQclear(testRes);
            PQfinish(_conn);
            _conn = nullptr;
            
            time_t now = time(nullptr);
            throw DatabaseConnectionException(ctime(&now), __FILE__, __LINE__, 
                                            typeid(*this).name(), __FUNCTION__, 
                                            _lastError.c_str());
        }
        PQclear(testRes);
        
        _lastError.clear();
    } catch (const DatabaseException& e) {
        _lastError = e.what();
        throw;
    } catch (const std::exception& e) {
        _lastError = "Unexpected error: ";
        _lastError += e.what();
        
        time_t now = time(nullptr);
        throw DatabaseConnectionException(ctime(&now), __FILE__, __LINE__, 
                                        typeid(*this).name(), __FUNCTION__, 
                                        _lastError.c_str());
    }
}

void PostgresqlConnection::Close() {
    if (_conn) {
        PQfinish(_conn);
        _conn = nullptr;
    }
}

void PostgresqlConnection::Execute(const std::string& sql, DatabaseCallback callback, void* data) {
    if (!IsOpen()) {
        time_t now = time(nullptr);
        throw DatabaseConnectionException(ctime(&now), __FILE__, __LINE__, 
                                        typeid(*this).name(), __FUNCTION__, 
                                        "Database is not open");
    }
    
    PGresult* res = PQexec(_conn, sql.c_str());
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
        _lastError = PQerrorMessage(_conn);
        PQclear(res);
        
        time_t now = time(nullptr);
        throw DatabaseConnectionException(ctime(&now), __FILE__, __LINE__, 
                                        typeid(*this).name(), __FUNCTION__, 
                                        _lastError.c_str());
    }
    
    if (callback && PQresultStatus(res) == PGRES_TUPLES_OK) {
        int nrows = PQntuples(res);
        int ncols = PQnfields(res);
        
        for (int row = 0; row < nrows; row++) {
            char** values = new char*[ncols];
            
            for (int col = 0; col < ncols; col++) {
                if (PQgetisnull(res, row, col)) {
                    values[col] = nullptr;
                } else {
                    values[col] = PQgetvalue(res, row, col);
                }
            }
            
            int result = callback(data, ncols, values, nullptr);
            
            delete[] values;
            
            if (result != 0) {
                break;
            }
        }
    }
    
    PQclear(res);
    _lastError.clear();
}

std::string PostgresqlConnection::GetLastError() const {
    return _lastError;
} 