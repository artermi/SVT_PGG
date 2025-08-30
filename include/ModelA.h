#ifndef MODELA_H
#define MODELA_H

#include <random>
#include <functional>
#include "RandomnessModel.h"

class ModelA : public RandomnessModel {
    std::function<double()> sampler;  // no internal rng anymore

public:
    // minimal change: accept rng_r by reference
    ModelA(double r0, double sigma, std::mt19937& rng_r);

    // minimal change: initialize now also takes rng_main
    void initialize(Lattice& lattice, std::mt19937& rng_main) override;

    void refresh(int t, Lattice& lattice) override;
};

#endif