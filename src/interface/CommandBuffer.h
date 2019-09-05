#pragma once

#include "ResourceTypes.h"
#include "Framebuffer.h"
#include <string>
#include "RenderPassCommandBuffer.h"
#include "ComputePassCommandBuffer.h"

namespace gfx
{
    class CommandBuffer
    {
    public:
        virtual RenderPassCommandBuffer* beginRenderPass(RenderPassId passId, const FrameBuffer& frameBuffer, const std::string& name) = 0;
        virtual void endRenderPass(RenderPassCommandBuffer* commandBuffer) = 0;
        
        virtual ComputePassCommandBuffer* beginComputePass(const std::string& name) { return nullptr; };
        virtual void endComputePass(ComputePassCommandBuffer* commandBuffer) {}
    };
}
