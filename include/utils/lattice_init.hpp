#pragma once
#include <cstdint>
#include "utils/splitmix.hpp"

// Assumes: grid.L, grid.set(x,y,state), and State::{C,D}
template<class Grid>
inline void build_initial_lattice_fn(Grid& grid, uint64_t seed, double p_init = 0.5) {
    for (int y = 0; y < grid.L; ++y) {
        for (int x = 0; x < grid.L; ++x) {
            const uint64_t key = (static_cast<uint64_t>(y) << 32) ^ static_cast<uint64_t>(x);
            const uint64_t z   = splitmix64(seed ^ 0xC0FFEEFACEB00B5ull ^ key);
            const bool is_C    = (u01_from_uint(z) < p_init);
            grid.set(x, y, is_C ? State::C : State::D);
        }
    }
}