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
    Simulation(const Config& config);
    void run();

private:
    Config cfg;
    Lattice lattice;
    std::unique_ptr<RandomnessModel> model;
    std::ofstream out_summary;
    std::string prefix;

    double log_snapshot(int t);
};