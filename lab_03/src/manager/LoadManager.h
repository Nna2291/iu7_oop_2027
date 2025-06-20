#pragma once
#include "DirectorSolution.h"
#include "ReaderSolution.h"
#include <cstdlib>
#include <memory>

#define LISTBONEMODELDIRECTORCREATOR_ID 1
#define MATRIXBONEMODELDIRECTORCREATOR_ID 2

#define TXTBONEMODELREADERCREATOR_ID 1
#define SQLITEBONEMODELREADERCREATOR_ID 2
#define POSTGRESQLBONEMODELREADERCREATOR_ID 3

class LoadManager {
    public:
        LoadManager();
        ~LoadManager() = default;

        std::shared_ptr<Object> LoadBoneModelFile(std::size_t directorID, std::size_t readerID, const char *filename);

    protected:
        DirectorSolution _dsolution;
        ReaderSolution _rsolution;
};
