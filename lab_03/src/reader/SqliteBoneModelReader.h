#pragma once
#include "BoneModelReader.h"
#include "CenterStrategy.h"
#include "DatabaseConnection.h"
#include <memory>

class SqliteBoneModelReader : public BoneModelReader {
    public:
        SqliteBoneModelReader(const char* filename);
        virtual ~SqliteBoneModelReader();

        virtual void Open();
        virtual void Close();
        virtual bool IsOpen();

        virtual std::vector<Point> ReadPoints();
        virtual std::vector<Edge> ReadEdges();
        virtual std::vector<Face> ReadFaces();
        virtual Point ReadCenter();

    protected:
        const char* _fname;
        DatabaseConnection* _dbConnection;
        std::shared_ptr<BaseCenterStrategy> _centerStrategy;
};