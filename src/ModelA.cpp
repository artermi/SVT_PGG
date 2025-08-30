#include "ModelA.h"


ModelA::ModelA(double r0, double sigma, std::mt19937& rng_r) {
    if (sigma == 0.0) {
        sampler = [=]() { return r0; };
    } else {
        std::uniform_real_distribution<> dist(r0 - std::sqrt(3) * sigma,
                                              r0 + std::sqrt(3) * sigma);
        // minimal change: sampler closes over rng_r, no internal rng
        sampler = [&rng_r, dist]() mutable { return dist(rng_r); };
    }
}

void ModelA::initialize(Lattice& lattice, std::mt19937& rng_main) {
    for (auto& g : lattice.get_groups()) {
        g.r_value = sampler();  // uses rng_r stream
    }
}

void ModelA::refresh(int, Lattice&) {
    // nothing: spatial randomness is frozen
}