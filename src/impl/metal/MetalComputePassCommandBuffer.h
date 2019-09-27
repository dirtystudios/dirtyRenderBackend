//
//  MetalComputePassCommandBuffer.h
//  RenderBackend
//
//  Created by Eugene Sturm on 8/29/19.
//

#pragma once

#import <Metal/Metal.h>
#include "ComputePassCommandBuffer.h"

namespace gfx
{
    class ResourceManager;

    class MetalComputePassCommandBuffer : public ComputePassCommandBuffer
    {
    private:
        id<MTLComputeCommandEncoder> _encoder;
        ResourceManager* _resourceManager;
    public:
        MetalComputePassCommandBuffer(id<MTLComputeCommandEncoder> encoder, ResourceManager* resourceManager);
        
        virtual void setPipelineState(PipelineStateId pipelineState) override;
        virtual void setBuffer(BufferId buffer, uint8_t index) override;
        virtual void setTexture(TextureId texture, uint8_t index) override;
        virtual void dispatch(std::array<size_t, 3> threadsPerGrid, std::array<size_t, 3> threadsPerGroup) override;
        
        id<MTLComputeCommandEncoder> getMTLEncoder() { return _encoder; }
    };
}
