#include "ModelB.h"
#include <limits>
#include <cmath>
#include <random>

ModelB::ModelB(double r0, double sigma, int lattice_size, int tau_, std::mt19937& rng_r)
    : tau((tau_ <= 0) ? std::numeric_limits<int>::max() : tau_) 
{
    if (sigma == 0.0) {
        sampler = [=]() { return r0; };
    } else {
        std::uniform_real_distribution<> dist(r0 - std::sqrt(3) * sigma,
                                              r0 + std::sqrt(3) * sigma);
        // minimal change: use rng_r passed in, no internal rng
        sampler = [&rng_r, dist]() mutable { return dist(rng_r); };
    }
}


void ModelB::refresh(int t, Lattice& lattice) {
    int block = t / tau;
    if (block == current_block) return;
    current_block = block;

    double new_r = sampler();
    for (auto& g : lattice.get_groups()) {
        g.r_value = new_r;
    }
}
void ModelB::initialize(Lattice& lattice, std::mt19937& rng_main) {
    refresh(0, lattice);  // Just like in your earlier version
}
