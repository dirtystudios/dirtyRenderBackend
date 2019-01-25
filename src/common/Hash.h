#pragma once

#include <functional>

inline void HashCombine(size_t& seed) {}

template <typename HashType, typename... HashArgs>
inline void HashCombine(size_t& seed, const HashType& v, HashArgs... args) {
    seed ^= std::hash<HashType>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    HashCombine(seed, args...);
}

template <typename... Args>
inline size_t HashCombine(Args... args) {
    size_t hash = 0;
    HashCombine(hash, args...);
    return hash;
}
