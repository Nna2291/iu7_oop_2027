#pragma once
#include "BaseSceneCommand.h"

class GetCameraIDsSceneCommand : public BaseSceneCommand {
    public:
        GetCameraIDsSceneCommand();
        virtual ~GetCameraIDsSceneCommand() = default;

        virtual void Execute() override;
        virtual std::string GetDescription() const override;
        std::vector<std::size_t> GetIDs();
    private:
        std::vector<std::size_t> _ids;
};