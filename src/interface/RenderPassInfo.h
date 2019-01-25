//
//  RenderPassInfo.h
//  planet
//
//  Created by Eugene Sturm on 6/20/18.
//

#pragma once

#include "AttachmentDesc.h"
#include <optional>

namespace gfx
{
    class RenderPassInfo
    {
    public:
        void addColorAttachment(const ColorAttachmentDesc& desc) { _colorAttachments.emplace_back(desc); }
        void setDepthAttachment(const DepthAttachmentDesc& desc) { _depthAttachment = desc; }
        void setStencilAttachment(const StencilAttachmentDesc& desc) { _stencilAttachment = desc; }
        
        const std::vector<ColorAttachmentDesc>& colorAttachments() const { return _colorAttachments; }
        const std::optional<DepthAttachmentDesc>& depthAttachment() const { return _depthAttachment; }
        const std::optional<StencilAttachmentDesc>& stencilAttachment() const { return _stencilAttachment; }
        bool hasDepth() const { return _depthAttachment.has_value(); }
        bool hasStencil() const { return _stencilAttachment.has_value(); }
        
    private:
        std::vector<ColorAttachmentDesc> _colorAttachments;
        std::optional<DepthAttachmentDesc> _depthAttachment;
        std::optional<StencilAttachmentDesc> _stencilAttachment;
    };
}
