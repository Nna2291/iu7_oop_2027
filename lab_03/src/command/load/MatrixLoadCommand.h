#pragma once
#include "BaseLoadCommand.h"


class MatrixLoadCommand : public BaseLoadCommand {
    public:
        MatrixLoadCommand();
        virtual ~MatrixLoadCommand() = default;
        virtual void Execute() override;
        virtual std::string GetDescription() const override;
};