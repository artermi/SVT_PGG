#ifndef MODELB_H
#define MODELB_H

#include <random>
#include <functional>
#include "RandomnessModel.h"

class ModelB : public RandomnessModel {
    std::mt19937 rng;
    std::function<double()> sampler;
    int tau;
    std::vector<double> current_r;

public:
    ModelB(double r0, double sigma, int lattice_size, int tau);
    void initialize(Lattice& lattice) override;
    void refresh(int t, Lattice& lattice) override;
};

#endif
