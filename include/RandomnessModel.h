#ifndef RANDOMNESSMODEL_H
#define RANDOMNESSMODEL_H

#include "Lattice.h"

class RandomnessModel {
public:
    virtual void initialize(Lattice& lattice, std::mt19937& rng_main) = 0;
    virtual void refresh(int t, Lattice& lattice) = 0;
    virtual ~RandomnessModel() = default;
};

#endif