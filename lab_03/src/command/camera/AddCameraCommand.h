#pragma once
#include "BaseCameraCommand.h"


class AddCameraCommand : public BaseCameraCommand {
    public:
        AddCameraCommand(const Point &pos);
        AddCameraCommand() = delete;
        virtual ~AddCameraCommand() = default;
        virtual void Execute() override;
        virtual std::string GetDescription() const override;

    private:
        Point _pos;
};