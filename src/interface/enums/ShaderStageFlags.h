#pragma once

#include <bitset>

namespace gfx {
    constexpr uint8_t ShaderStageBitPositionVertex = 0;
    constexpr uint8_t ShaderStageBitPositionTessControl = 1;
    constexpr uint8_t ShaderStageBitPositionTessEval = 2;
    constexpr uint8_t ShaderStageBitPositionCompute = 3;
    constexpr uint8_t ShaderStageBitPositionPixel = 4;
    
    constexpr uint8_t ShaderStageBitVertex = 1 << ShaderStageBitPositionVertex;
    constexpr uint8_t ShaderStageBitTessControl = 1 << ShaderStageBitPositionTessControl;
    constexpr uint8_t ShaderStageBitTessEval = 1 << ShaderStageBitPositionTessEval;
    constexpr uint8_t ShaderStageBitCompute = 1 << ShaderStageBitPositionCompute;
    constexpr uint8_t ShaderStageBitPixel = 1 << ShaderStageBitPositionPixel;
    
    constexpr uint8_t ShaderStageBitAllStages = 0x1f;
    
    constexpr size_t kShaderStageBitPositionCount = 5;
    using ShaderStageFlags = std::bitset<kShaderStageBitPositionCount>;
}
