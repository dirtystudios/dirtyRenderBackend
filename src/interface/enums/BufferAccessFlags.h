#pragma once

#include <bitset>

namespace gfx {
    constexpr uint8_t BufferAccessBitPositionGpuRead = 0;
    constexpr uint8_t BufferAccessBitPositionGpuWrite = 1;
    constexpr uint8_t BufferAccessBitPositionCpuRead = 2;
    constexpr uint8_t BufferAccessBitPositionCpuWrite = 3;
    
    constexpr uint8_t BufferAccessBitGpuRead = 1 << BufferAccessBitPositionGpuRead;
    constexpr uint8_t BufferAccessBitGpuWrite = 1 << BufferAccessBitPositionGpuWrite;
    constexpr uint8_t BufferAccessBitCpuRead = 1 << BufferAccessBitPositionCpuRead;
    constexpr uint8_t BufferAccessBitCpuWrite = 1 << BufferAccessBitPositionCpuWrite;
    
    constexpr uint8_t BufferAccessBitGpuReadCpuWrite = BufferAccessBitGpuRead | BufferAccessBitCpuWrite;
    
    constexpr size_t kBufferAccessBitPositionCount = 4;
    using BufferAccessFlags = std::bitset<kBufferAccessBitPositionCount>;
}

