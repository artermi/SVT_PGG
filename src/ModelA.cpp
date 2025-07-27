#include "ModelA.h"

ModelA::ModelA(double r0, double sigma) {
    rng.seed(42);
    std::uniform_real_distribution<> dist(r0 - std::sqrt(3)*sigma, r0 + std::sqrt(3)*sigma);
    sampler = [=]() mutable { return dist(rng); };
}

void ModelA::initialize(Lattice& lattice) {
    for (auto& g : lattice.get_groups()) {
        g.r_value = sampler();
    }
}

void ModelA::refresh(int, Lattice&) {}