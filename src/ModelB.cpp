#include "ModelB.h"

ModelB::ModelB(double r0, double sigma, int lattice_size, int tau)
    : tau(tau) {

    rng.seed(42);

    if (sigma == 0.0) {
        sampler = [=]() { return r0; };
    } else {
        std::uniform_real_distribution<> dist(r0 - std::sqrt(3)*sigma, r0 + std::sqrt(3)*sigma);
        sampler = [=]() mutable { return dist(rng); };
    }

    this->tau = (tau <= 0) ? std::numeric_limits<int>::max() : tau;
}


void ModelB::refresh(int t, Lattice& lattice) {
    int block = t / tau;
    if (block == current_block) return; // already refreshed for this block
    current_block = block;

    double new_r = sampler();
    for (auto& g : lattice.get_groups()) {
        g.r_value = new_r;
    }
}

void ModelB::initialize(Lattice& lattice) {
    refresh(0, lattice);  // Just like in your earlier version
}
