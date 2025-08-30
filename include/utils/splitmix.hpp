#pragma once
#include <cstdint>

// SplitMix64: fast mixer for seeds/hashes
inline uint64_t splitmix64(uint64_t x) {
    x += 0x9E3779B97F4A7C15ull;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ull;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBull;
    return x ^ (x >> 31);
}

// Convert 64-bit int to uniform [0,1)
inline double u01_from_uint(uint64_t z) {
    return (z >> 11) * (1.0 / 9007199254740992.0); // 2^53
}