#pragma once

#include <stdint.h>
#include <functional>
#include <unordered_map>
#include <string>

namespace gfx {
    enum class ShaderType : uint8_t {
        VertexShader = 0,
        TessControlShader,
        TessEvalShader,
        PixelShader,
        ComputeShader,
        Count,
    };
}
