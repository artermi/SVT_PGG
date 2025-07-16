#ifndef MODELA_H
#define MODELA_H

#include <random>
#include <functional>
#include "RandomnessModel.h"

class ModelA : public RandomnessModel {
    std::mt19937 rng;
    std::function<double()> sampler;

public:
    ModelA(double r0, double sigma);
    void initialize(Lattice& lattice) override;
    void refresh(int t, Lattice& lattice) override;
};

#endif