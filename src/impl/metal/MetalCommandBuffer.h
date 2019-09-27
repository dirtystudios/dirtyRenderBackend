//
//  MetalCommandBuffer.h
//  planet
//
//  Created by Eugene Sturm on 5/25/18.
//

#pragma once

#include <Metal/Metal.h>
#include "RenderPassCommandBuffer.h"
#include "ComputePassCommandBuffer.h"
#include "Framebuffer.h"
#include "CommandBuffer.h"

namespace gfx
{
    class ResourceManager;
    class MetalRenderPass;
    class MetalRenderPassCommandBuffer;
    class MetalComputePassCommandBuffer;
    
    class MetalCommandBuffer : public CommandBuffer
    {
    private:
        id<MTLCommandBuffer> _commandBuffer;
        ResourceManager* _resourceManager { nullptr };
        std::unique_ptr<MetalRenderPassCommandBuffer> _currentGraphicsPass { nullptr };
        std::unique_ptr<MetalComputePassCommandBuffer> _currentComputePass { nullptr };
    public:
        MetalCommandBuffer(id<MTLCommandBuffer> commandBuffer, ResourceManager* resourceManager);
        
        virtual RenderPassCommandBuffer* beginRenderPass(RenderPassId passId, const FrameBuffer& frameBuffer, const std::string& name = "") override;
        virtual void endRenderPass(RenderPassCommandBuffer* commandBuffer) override;
        
        virtual ComputePassCommandBuffer* beginComputePass(const std::string& name) override;
        virtual void endComputePass(ComputePassCommandBuffer* commandBuffer) override;
        
        id<MTLCommandBuffer> getMTLCommandBuffer();
        void commit();
        MTLRenderPassDescriptor* getMTLRenderPassDescriptor(MetalRenderPass* renderPass, const FrameBuffer& frameBuffer);
    };
    
    
}
