//
//  ComputePassCommandBuffer.hpp
//  RenderBackend
//
//  Created by Eugene Sturm on 8/29/19.
//

#pragma once
#include "ResourceTypes.h"
#include <array>

namespace gfx
{
    class ComputePassCommandBuffer
    {
    public:
        virtual void setPipelineState(PipelineStateId pipelineState) {};
        virtual void setBuffer(BufferId buffer, uint8_t index) {};
        virtual void setTexture(TextureId texture, uint8_t index) {};
        virtual void dispatch(std::array<size_t, 3> threadsPerGrid, std::array<size_t, 3> threadsPerGroup) {}
    };
}
