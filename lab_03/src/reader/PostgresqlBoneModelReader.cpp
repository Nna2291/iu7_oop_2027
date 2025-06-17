#include "PostgresqlBoneModelReader.h"
#include "sqlitereaderexception.h"
#include "databaseexception.h"
#include "DatabaseFactoryCreator.h"
#include <memory>
#include <sstream>

static const char * ReadPointsSql = "SELECT * FROM point ORDER BY id ASC";
static const char * ReadEdgesSql = "SELECT * FROM edge";

PostgresqlBoneModelReader::PostgresqlBoneModelReader(const std::string& filename) 
    : _fname(filename), _dbConnection(nullptr), _centerStrategy(std::make_shared<ClassicCenterStrategy>()) {
    
    auto factory = DatabaseFactoryCreator::CreateFactory(DB_POSTGRESQL);
    _dbConnection = factory->create().release();
}

PostgresqlBoneModelReader::~PostgresqlBoneModelReader() { 
    if (IsOpen())
        Close();
    delete _dbConnection;
}

bool PostgresqlBoneModelReader::IsOpen() {
    return _dbConnection && _dbConnection->IsOpen();
}

void PostgresqlBoneModelReader::Open() {
    try {
        _dbConnection->Open(_fname.c_str());
    } catch (const DatabaseException& e) {
        time_t now = time(nullptr);
        throw SqliteNoFileException(ctime(&now), __FILE__, __LINE__, typeid(*this).name(), __FUNCTION__);
    }
}

void PostgresqlBoneModelReader::Close() {
    if (_dbConnection) {
        _dbConnection->Close();
    }
}

static int ReadPointsCallback (void* data, int argc, char** argv, char **colNames) {
    (void) colNames;
    auto points = static_cast<std::vector<Point>*>(data);
    if (argc != 4) {
        return 1;
    }
    double x = std::atof(argv[1]);
    double y = std::atof(argv[2]);
    double z = std::atof(argv[3]);
    points->push_back(Point(x, y, z));
    return 0;
}

std::vector<Point> PostgresqlBoneModelReader::ReadPoints() {
    std::vector<Point> points;
    
    if (!IsOpen()) {
        return points;
    }
    
    try {
        _dbConnection->Execute(ReadPointsSql, ReadPointsCallback, &points);
    } catch (const DatabaseException& e) {
        time_t now = time(nullptr);
        throw SqliteReadException(ctime(&now), __FILE__, __LINE__, typeid(*this).name(), __FUNCTION__);
    }
    return points;
}

static int ReadEdgesCallback (void* data, int argc, char** argv, char **colNames) {
    (void) colNames;
    auto edges = static_cast<std::vector<Edge>*>(data);
    if (argc != 3) {
        return 1;
    }
    int p1 = std::atoi(argv[1]) - 1;
    int p2 = std::atoi(argv[2]) - 1;
    if (p1 < 0 || p2 < 0) {
        return 1;
    }
    edges->push_back(Edge(p1, p2));
    return 0;
}

std::vector<Edge> PostgresqlBoneModelReader::ReadEdges() {
    std::vector<Edge> edges;
    
    if (!IsOpen()) {
        return edges;
    }
    
    try {
        _dbConnection->Execute(ReadEdgesSql, ReadEdgesCallback, &edges);
    } catch (const DatabaseException& e) {
        time_t now = time(nullptr);
        throw SqliteReadException(ctime(&now), __FILE__, __LINE__, typeid(*this).name(), __FUNCTION__);
    }

    return edges;
}

std::vector<Face> PostgresqlBoneModelReader::ReadFaces() {
    return std::vector<Face>();
}

Point PostgresqlBoneModelReader::ReadCenter() {
    std::vector<Point> points = ReadPoints();
    return _centerStrategy->CenterAlgorithm(points);
} 