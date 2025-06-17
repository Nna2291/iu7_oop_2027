#include "DrawManager.h"
#include "ToggleFaceCullingCommand.h"

void DrawManager::SetFaceCullingForVisitor(DrawVisitor& visitor) {
    visitor.SetFaceCullingEnabled(ToggleFaceCullingCommand::GetFaceCullingEnabled());
} 