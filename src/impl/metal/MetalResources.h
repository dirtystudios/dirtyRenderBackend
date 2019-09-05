//
//  MetalResources.h
//  planet
//
//  Created by Eugene Sturm on 8/8/16.
//

#pragma once

#import <Metal/Metal.h>
#include "BufferDesc.h"
#include "PipelineStateDesc.h"
#include "PixelFormat.h"
#include "Resource.h"
#include "ShaderType.h"
#include "VertexLayoutDesc.h"
#include "RenderPassInfo.h"

namespace gfx {

    struct MetalBuffer : public Resource {
        ~MetalBuffer() {}

        BufferDesc    desc;
        id<MTLBuffer> mtlBuffer;
    };

    struct MetalLibraryFunction : public Resource {
        ~MetalLibraryFunction() {}

        id<MTLLibrary>  mtlLibrary;
        id<MTLFunction> mtlFunction;
        std::string     functionName;
        ShaderType      type;
    };

    struct MetalVertexLayout : public Resource {
        ~MetalVertexLayout() { }

        VertexLayoutDesc     layoutDesc;
        MTLVertexDescriptor* mtlVertexDesc;
    };

    struct MetalPipelineState : public Resource {
        ~MetalPipelineState() {}

        id<MTLRenderPipelineState>   mtlPipelineState;
        id<MTLDepthStencilState>     mtlDepthStencilState;
        PipelineStateDesc            pipelineStateDesc;
        MTLRenderPipelineReflection* reflection;
    };

    struct MetalComputePipelineState : public Resource {
        ~MetalComputePipelineState() {}
        
        id<MTLComputePipelineState> mtlComputePipelineState;
        ComputePipelineStateDesc desc;
        MTLAutoreleasedComputePipelineReflection* reflection;
    };

    struct MetalTexture : public Resource {
        ~MetalTexture() {}

        id<MTLTexture>      mtlTexture { nil };
        id<MTLSamplerState> mtlSamplerState { nil };
        PixelFormat         externalFormat { PixelFormat::Invalid };
        uint32_t            bytesPerRow { 0 };
        uint32_t            bytesPerImage { 0 };
    };
    
    struct MetalRenderPass : public Resource {
        RenderPassInfo info;
    };
}
