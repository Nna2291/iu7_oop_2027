#include "SqliteBoneModelReader.h"
#include "sqlitereaderexception.h"
#include "databaseexception.h"
#include "DatabaseFactoryCreator.h"
#include <memory>

static const char * ReadPointsSql = "SELECT * FROM point ORDER BY id ASC";
static const char * ReadEdgesSql = "SELECT * FROM edge";

SqliteBoneModelReader::SqliteBoneModelReader(const char *filename) 
    : _fname(filename), _dbConnection(nullptr), _centerStrategy(std::make_shared<ClassicCenterStrategy>()) {
    
    auto factory = DatabaseFactoryCreator::CreateFactory(DB_SQLITE);
    _dbConnection = factory->create().release();
}

SqliteBoneModelReader::~SqliteBoneModelReader() { 
    if (IsOpen())
        Close();
    delete _dbConnection;
}

bool SqliteBoneModelReader::IsOpen() {
    return _dbConnection && _dbConnection->IsOpen();
}

void SqliteBoneModelReader::Open() {
    try {
        _dbConnection->Open(_fname);
    } catch (const DatabaseException& e) {
        time_t now = time(nullptr);
        throw SqliteNoFileException(ctime(&now), __FILE__, __LINE__, typeid(*this).name(), __FUNCTION__);
    }
}

void SqliteBoneModelReader::Close() {
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

std::vector<Point> SqliteBoneModelReader::ReadPoints() {
    if (!IsOpen()) {
        return std::vector<Point>();
    }

    std::vector<Point> points;

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

std::vector<Edge> SqliteBoneModelReader::ReadEdges() {
    if (!IsOpen()) {
        return std::vector<Edge>();
    }
    
    std::vector<Edge> edges;
    try {
        _dbConnection->Execute(ReadEdgesSql, ReadEdgesCallback, &edges);
    } catch (const DatabaseException& e) {
        time_t now = time(nullptr);
        throw SqliteReadException(ctime(&now), __FILE__, __LINE__, typeid(*this).name(), __FUNCTION__);
    }

    return edges;
}

std::vector<Face> SqliteBoneModelReader::ReadFaces() {
    return std::vector<Face>();
}

Point SqliteBoneModelReader::ReadCenter() {
    std::vector<Point> points = ReadPoints();
    return _centerStrategy->CenterAlgorithm(points);
}
