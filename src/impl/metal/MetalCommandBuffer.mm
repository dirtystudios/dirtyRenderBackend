//
//  MetalCommandBuffer.mm
//  planet
//
//  Created by Eugene Sturm on 5/25/18.
//

#include "MetalCommandBuffer.h"
#include "ResourceManager.h"
#include "MetalResources.h"
#include "MetalRenderPassCommandBuffer.h"
#include "MetalComputePassCommandBuffer.h"
#include "MetalEnumAdapter.h"

using namespace gfx;

MetalCommandBuffer::MetalCommandBuffer(id<MTLCommandBuffer> commandBuffer, ResourceManager* resourceManager)
: _commandBuffer(commandBuffer)
, _resourceManager(resourceManager)
{}

RenderPassCommandBuffer* MetalCommandBuffer::beginRenderPass(RenderPassId passId, const FrameBuffer& frameBuffer, const std::string& name)
{
    if (_currentGraphicsPass || _currentComputePass) {
        assert(false);
    }
    
    
    // TODO: Validate RenderPassId is valid with FrameBuffer
    MetalRenderPass* renderPass = _resourceManager->GetResource<MetalRenderPass>(passId);
    dg_assert_nm(renderPass);
    if (renderPass == nullptr) {
        return nullptr;
    }
    
    MTLRenderPassDescriptor* renderPassDesc = getMTLRenderPassDescriptor(renderPass, frameBuffer);
    
    id<MTLRenderCommandEncoder> encoder = [_commandBuffer renderCommandEncoderWithDescriptor:renderPassDesc];
    encoder.label = [NSString stringWithUTF8String:renderPass->info.label().c_str()];
    _currentGraphicsPass.reset(new MetalRenderPassCommandBuffer(encoder, _resourceManager));
    
    return _currentGraphicsPass.get();
}

void MetalCommandBuffer::endRenderPass(RenderPassCommandBuffer* commandBuffer)
{
    if (commandBuffer != _currentGraphicsPass.get()) {
        return;
    }
    
    id<MTLRenderCommandEncoder> encoder = _currentGraphicsPass->getMTLEncoder();
    [encoder endEncoding];
    _currentGraphicsPass.reset();
}

id<MTLCommandBuffer> MetalCommandBuffer::getMTLCommandBuffer()
{
    return _commandBuffer;
}

void MetalCommandBuffer::commit()
{
    dg_assert_nm(_currentGraphicsPass == nullptr);
    [_commandBuffer commit];
}

MTLRenderPassDescriptor* MetalCommandBuffer::getMTLRenderPassDescriptor(MetalRenderPass* renderPass, const FrameBuffer& frameBuffer)
{
    MTLRenderPassDescriptor* renderPassDesc = [MTLRenderPassDescriptor new];
    
    const RenderPassInfo& info = renderPass->info;
    for (const ColorAttachmentDesc& attachment : info.colorAttachments()) {
        dg_assert_nm(attachment.index < kMaxFramebufferColorAttachments && frameBuffer.color(attachment.index) != NULL_ID);
        if (attachment.index >= kMaxFramebufferColorAttachments || frameBuffer.color(attachment.index) == NULL_ID) {
            continue;
        }
        
        MetalTexture* texture = _resourceManager->GetResource<MetalTexture>(frameBuffer.color(attachment.index));
        dg_assert_nm(texture);
        if (texture) {
            dg_assert_nm(attachment.format == texture->externalFormat);
            MTLRenderPassColorAttachmentDescriptor* colorAttachment = [MTLRenderPassColorAttachmentDescriptor new];
            colorAttachment.texture = texture->mtlTexture;;
            colorAttachment.loadAction = MetalEnumAdapter::toMTL(attachment.loadAction);
            colorAttachment.storeAction = MetalEnumAdapter::toMTL(attachment.storeAction);
            colorAttachment.clearColor = MTLClearColorMake(attachment.clearColor[0], attachment.clearColor[1], attachment.clearColor[2], attachment.clearColor[3]);
            
            [renderPassDesc.colorAttachments setObject:colorAttachment atIndexedSubscript:attachment.index];
        }
    }
    
    if (info.depthAttachment().has_value()) {
        MetalTexture* texture = _resourceManager->GetResource<MetalTexture>(frameBuffer.depth());
        const DepthAttachmentDesc& attachment = info.depthAttachment().value();
        if (texture) {
            MTLRenderPassDepthAttachmentDescriptor* depthAttachment = [MTLRenderPassDepthAttachmentDescriptor new];
            depthAttachment.texture = texture->mtlTexture;
            depthAttachment.loadAction = MetalEnumAdapter::toMTL(attachment.loadAction);
            depthAttachment.storeAction = MetalEnumAdapter::toMTL(attachment.storeAction);
            depthAttachment.clearDepth = attachment.clearDepth;
            
            renderPassDesc.depthAttachment = depthAttachment;
        }
    }
    
    if (info.stencilAttachment().has_value()) {
        MetalTexture* texture = _resourceManager->GetResource<MetalTexture>(frameBuffer.stencil());
        const StencilAttachmentDesc& attachment = info.stencilAttachment().value();
        if (texture) {
            MTLRenderPassStencilAttachmentDescriptor* stencil = [MTLRenderPassStencilAttachmentDescriptor new];
            stencil.texture = texture->mtlTexture;
            stencil.loadAction = MetalEnumAdapter::toMTL(attachment.loadAction);
            stencil.storeAction = MetalEnumAdapter::toMTL(attachment.storeAction);
            stencil.clearStencil = attachment.clearStencil;
            
            renderPassDesc.stencilAttachment = stencil;
        }
    }
    
    return renderPassDesc;
}

ComputePassCommandBuffer* MetalCommandBuffer::beginComputePass(const std::string& name)
{
    if (_currentGraphicsPass || _currentComputePass) {
        assert(false);
    }
    
    id<MTLComputeCommandEncoder> encoder = [_commandBuffer computeCommandEncoder];
    encoder.label = [NSString stringWithUTF8String:name.c_str()];
    _currentComputePass.reset(new MetalComputePassCommandBuffer(encoder, _resourceManager));
    
    return _currentComputePass.get();
}

void MetalCommandBuffer::endComputePass(ComputePassCommandBuffer* commandBuffer)
{
    if (commandBuffer != _currentComputePass.get()) {
        return;
    }
    
    id<MTLComputeCommandEncoder> encoder = _currentComputePass->getMTLEncoder();
    [encoder endEncoding];
    _currentComputePass.reset();
}
