#pragma once
#include "BaseDrawCommand.h"

class ToggleFaceCullingCommand : public BaseDrawCommand {
public:
    ToggleFaceCullingCommand() = default;
    virtual void Execute() override;
    virtual std::string GetDescription() const override;
    
    static bool GetFaceCullingEnabled() { return _faceCullingEnabled; }

private:
    static bool _faceCullingEnabled;
}; 