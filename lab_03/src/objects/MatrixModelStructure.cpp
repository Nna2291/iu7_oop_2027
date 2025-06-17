#include "MatrixModelStructure.h"
#include "modelexception.h"

MatrixModelStructure::MatrixModelStructure() : _center(Point()), _points(std::vector<Point>()), _edgeMatrix(SquareMatrix<int>()), _faces(std::vector<Face>()) {};

void MatrixModelStructure::Transform(const TransformAction& action) {
    for (Point& point : _points) {
        action.TransformPoint(point);
    }
    action.TransformPoint(_center);
}

std::vector<Point> MatrixModelStructure::GetPoints() const {
    return _points;
}

std::vector<Edge> MatrixModelStructure::GetEdges() const {
    std::vector<Edge> _edges;
    for (size_t i = 0; i < _edgeMatrix.size(); ++i) {
        for (size_t j = i + 1; j < _edgeMatrix[i].size(); ++j) {
            if (_edgeMatrix[i][j]) {
                _edges.push_back(Edge(i, j));
            }
        }
    }
    return _edges;
}

std::vector<Face> MatrixModelStructure::GetFaces() const {
    return _faces;
}

Point MatrixModelStructure::GetCenter() const {
    Point copy(_center);
    return copy;
}

void MatrixModelStructure::SetCenter(const Point& center) {
    _center = center;
}

void MatrixModelStructure::AddPoint(const Point& point) {
    _points.push_back(point);
    _edgeMatrix.Resize(_points.size(), false);
}

void MatrixModelStructure::AddEdge(const Edge& edge) {
    if (edge.GetFirst() >= _points.size() || edge.GetSecond() >= _points.size()) {
        time_t now = time(nullptr);
        throw EdgeOutOfPointsException(ctime(&now), __FILE__, __LINE__, typeid(*this).name(), __FUNCTION__);
    }
    *(_edgeMatrix[edge.GetFirst()].begin() + edge.GetSecond()) = true;
    *(_edgeMatrix[edge.GetSecond()].begin() + edge.GetFirst()) = true;
}

void MatrixModelStructure::AddFace(const Face& face) {
    _faces.push_back(face);
}

void MatrixModelStructure::ClearPoints() {
    _points.clear();
}

std::shared_ptr<ModelStructure> MatrixModelStructure::Clone() const {
    auto cloned = std::make_shared<MatrixModelStructure>();
    cloned->SetCenter(_center);
    for (const Point& point : _points) {
        cloned->AddPoint(point);
    }
    cloned->_edgeMatrix = _edgeMatrix;

    for (const Face& face : _faces) {
        cloned->AddFace(face);
    }

    return cloned;
}
