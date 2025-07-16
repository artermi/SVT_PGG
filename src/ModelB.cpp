#include "ModelB.h"

ModelB::ModelB(double r0, double sigma, int lattice_size, int tau)
    : tau(tau), current_r(lattice_size * lattice_size, r0) {
    rng.seed(42);
    std::uniform_real_distribution<> dist(r0 - std::sqrt(3)*sigma, r0 + std::sqrt(3)*sigma);
    sampler = [=]() mutable { return dist(rng); };
}

void ModelB::initialize(Lattice& lattice) {
    refresh(0, lattice);
}

void ModelB::refresh(int t, Lattice& lattice) {
    if (t % tau != 0) return;
    double new_r = sampler();
    for (auto& g : lattice.get_groups()) {
        g.r_value = new_r;
    }
}
