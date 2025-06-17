#pragma once
#include "BoneModelReader.h"
#include "CenterStrategy.h"
#include "DatabaseConnection.h"
#include <memory>
#include <string>

class PostgresqlBoneModelReader : public BoneModelReader {
    public:
        PostgresqlBoneModelReader(const std::string& filename);
        virtual ~PostgresqlBoneModelReader();

        virtual void Open();
        virtual void Close();
        virtual bool IsOpen();

        virtual std::vector<Point> ReadPoints();
        virtual std::vector<Edge> ReadEdges();
        virtual std::vector<Face> ReadFaces();
        virtual Point ReadCenter();

    protected:
        std::string _fname;
        DatabaseConnection* _dbConnection;
        std::shared_ptr<BaseCenterStrategy> _centerStrategy;
}; 