//
//  AttachmentDesc.h
//  planet
//
//  Created by Eugene Sturm on 6/20/18.
//

#pragma once

#include "PixelFormat.h"
#include "LoadAction.h"
#include "StoreAction.h"
#include <array>

namespace gfx
{
    struct AttachmentDesc
    {
        PixelFormat format { PixelFormat::Invalid };
        LoadAction loadAction { LoadAction::DontCare };
        StoreAction storeAction { StoreAction::DontCare };
        
    };
    
    struct ColorAttachmentDesc : public AttachmentDesc
    {
        uint8_t index { 0 };
        std::array<float, 4> clearColor { { 0, 0, 0, 1} };
    };
    
    struct DepthAttachmentDesc : public AttachmentDesc
    {
        float clearDepth { 0.f };
    };
    
    struct StencilAttachmentDesc : public AttachmentDesc
    {
        uint32_t clearStencil { 0 };
    };
}
