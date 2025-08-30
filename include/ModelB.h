#ifndef MODELB_H
#define MODELB_H

#include <random>
#include <functional>
#include "RandomnessModel.h"

class ModelB : public RandomnessModel {
    std::function<double()> sampler;
    int tau;
    int current_block = -1;
    double current_r;

public:
    ModelB(double r0, double sigma, int lattice_size, int tau, std::mt19937& rng_r);
    void initialize(Lattice& lat, std::mt19937& rng_main) override;
    void refresh(int t, Lattice& lattice) override;
};

#endif
