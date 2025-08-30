// Simulation.h
#pragma once
#include "Lattice.h"
#include "ModelA.h"
#include "ModelB.h"
#include "Config.h"
#include <fstream>
#include <memory>

class Simulation {
public:
    Simulation(const Config& cfg,
               std::mt19937& rng_main,
               std::mt19937& rng_r);
    void run();

private:
    Config cfg;
    Lattice lattice;
    std::unique_ptr<RandomnessModel> model;
    std::ofstream out_summary;
    std::string prefix;

    std::mt19937& rng_main;  // reference to main RNG
    std::mt19937& rng_r;     // reference to r RNG

    double log_snapshot(int t);
};