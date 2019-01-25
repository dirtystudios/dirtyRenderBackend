//
//  Framebuffer.h
//  planet
//
//  Created by Eugene Sturm on 6/22/18.
//

#pragma once

#include "ResourceTypes.h"
#include <array>
#include "DGAssert.h"

namespace gfx
{
    constexpr uint8_t kMaxFramebufferColorAttachments = 16;
    using ColorAttachmentArray = std::array<TextureId, kMaxFramebufferColorAttachments>;
    class FrameBuffer
    {
    public:
        FrameBuffer()
        {
            memset(_color.data(), 0, sizeof(_color));
        }
        
        void setColorAttachment(TextureId texture, uint8_t index)
        {
            dg_assert_nm(_color[index] == 0);
            _color[index] = texture;
        }
        void setDepthAttachment(TextureId texture) { _depth = texture; }
        void setStencilAttachment(TextureId texture) { _stencil = texture; }
        
        TextureId color(uint8_t index) const { dg_assert_nm(index < kMaxFramebufferColorAttachments && _color[index] != NULL_ID); return _color[index]; }
        TextureId depth() const { dg_assert_nm(_depth != NULL_ID); return _depth; };
        TextureId stencil() const { dg_assert_nm(_stencil != NULL_ID); return _stencil; };
    private:
        ColorAttachmentArray _color;
        TextureId _depth { 0 };
        TextureId _stencil { 0 };
    };
}
