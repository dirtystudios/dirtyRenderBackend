//
//  TextureUsage.h
//  planet
//
//  Created by Eugene Sturm on 6/22/18.
//

#pragma once

#include <bitset>

namespace gfx
{
    constexpr uint8_t TextureUsageBitPositionShaderRead = 0;
    constexpr uint8_t TextureUsageBitPositionShaderWrite = 1;
    constexpr uint8_t TextureUsageBitPositionRenderTarget = 2;
    
    constexpr uint8_t TextureUsageBitShaderRead = 1 << TextureUsageBitPositionShaderRead;
    constexpr uint8_t TextureUsageBitShaderWrite = 1 << TextureUsageBitPositionShaderWrite;
    constexpr uint8_t TextureUsageBitRenderTarget = 1 << TextureUsageBitPositionRenderTarget;
    
    constexpr size_t kTextureUsageBitPositionCount = 3;
    using TextureUsageFlags = std::bitset<kTextureUsageBitPositionCount>;
}
