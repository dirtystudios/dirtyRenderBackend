//
//  MetalComputePassCommandBuffer.mm
//  RenderBackend
//
//  Created by Eugene Sturm on 8/29/19.
//

#include "MetalComputePassCommandBuffer.h"
#include "ResourceManager.h"
#include "MetalResources.h"
using namespace gfx;

MetalComputePassCommandBuffer::MetalComputePassCommandBuffer(id<MTLComputeCommandEncoder> encoder, ResourceManager* resourceManager)
: _encoder(encoder)
, _resourceManager(resourceManager)
{    
}

void MetalComputePassCommandBuffer::setPipelineState(PipelineStateId pipelineState)
{
    MetalComputePipelineState* ps = _resourceManager->GetResource<MetalComputePipelineState>(pipelineState);
    [_encoder setComputePipelineState:ps->mtlComputePipelineState];
}
void MetalComputePassCommandBuffer::setBuffer(BufferId buffer, uint8_t index)
{
    MetalBuffer* buf = _resourceManager->GetResource<MetalBuffer>(buffer);
    [_encoder setBuffer:buf->mtlBuffer offset:0 atIndex:index];
}
void MetalComputePassCommandBuffer::setTexture(TextureId texture, uint8_t index)
{
    MetalTexture* tex = _resourceManager->GetResource<MetalTexture>(texture);
    [_encoder setTexture:tex->mtlTexture atIndex:index];
    [_encoder setSamplerState:tex->mtlSamplerState atIndex:index];
}
void MetalComputePassCommandBuffer::dispatch(std::array<size_t, 3> threadsPerGrid, std::array<size_t, 3> threadsPerGroup)
{
    [_encoder dispatchThreads:MTLSizeMake(threadsPerGrid[0], threadsPerGrid[1], threadsPerGrid[2])
        threadsPerThreadgroup:MTLSizeMake(threadsPerGroup[0], threadsPerGroup[1], threadsPerGroup[2])];
}
