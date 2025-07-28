// Simulation.cpp
#include "Simulation.h"
#include "SimulationUtils.h"
#include <filesystem>
#include <iomanip>
#include <iostream>

Simulation::Simulation(const Config& config)
    : cfg(config), lattice(config.L) {
    std::filesystem::create_directories("data");

    prefix = "r" + std::to_string(int(cfg.r0 * 10)) +
             "v" + std::to_string(int(cfg.sigma * 100)) +
             "_" + cfg.ftype +
             "_L" + std::to_string(cfg.L);

    if (cfg.use_model_b) {
        model = std::make_unique<ModelB>(cfg.r0, cfg.sigma, cfg.L, cfg.tau);
        prefix += "_Btau" + std::to_string(cfg.tau);
    } else {
        model = std::make_unique<ModelA>(cfg.r0, cfg.sigma);
        prefix += "_A";
    }

    model->initialize(lattice);

    out_summary.open("data/" + prefix + "_summary.csv");
    out_summary << "time,C,mean_payoff,var_payoff\n";

    std::cout << std::left
              << std::setw(6) << "time"
              << std::setw(10) << "C ratio"
              << std::setw(15) << "mean"
              << std::setw(15) << "var"
              << std::endl;
}

void Simulation::run() {
    for (int t = 0; t < cfg.steps; ++t) {
        model->refresh(t, lattice);

        if (t % cfg.snapshot_interval == 0) {
            double c = log_snapshot(t);
            if (c < 1e-6 || c > 1.0 - 1e-6) {
                std::cout << "[Terminated early at t = " << t << "] Fixation reached.\n";
                break;
            }
        }

        for (int i = 0; i < cfg.L * cfg.L; ++i){
            fermi_update(lattice, cfg.K);
        }
    }

    // Final refresh and snapshot at t = cfg.steps
    model->refresh(cfg.steps, lattice);      // Optional, if r(t) matters at t = T
    log_snapshot(cfg.steps);

    save_metadata(prefix, 0.0, "none");
    out_summary.close();
    std::cout << "Simulation complete." << std::endl;
}

double Simulation::log_snapshot(int t) {
    double c = lattice.get_strategy_ratio();
    double mean = mean_payoff(lattice);
    double var = var_payoff(lattice, mean);

    out_summary << t << "," << c << "," << mean << "," << var << "\n";

    std::cout << std::left
              << std::setw(6)  << t
              << std::setw(10) << std::fixed << std::setprecision(4) << c
              << std::setw(15) << mean
              << std::setw(15) << var
              << '\n';

    return c;
}