// Simulation.cpp
#include "Simulation.h"
#include "SimulationUtils.h"
#include <filesystem>
#include <iomanip>
#include <iostream>

Simulation::Simulation(const Config& config,
                       std::mt19937& rng_main_,
                       std::mt19937& rng_r_)
    : cfg(config), lattice(config.L),
      rng_main(rng_main_), rng_r(rng_r_) {

    prefix = format_prefix(cfg.r0, cfg.sigma, cfg.ftype, cfg.L,
                           cfg.model_type == ModelType::TEMPORAL, cfg.tau);

    if (cfg.model_type == ModelType::TEMPORAL) {
        model = std::make_unique<ModelB>(cfg.r0, cfg.sigma, cfg.L, cfg.tau, rng_r);
    } else {
        model = std::make_unique<ModelA>(cfg.r0, cfg.sigma, rng_r);
    }

    model->initialize(lattice, rng_main);

    out_summary.open(prefix + "_summary.csv");
    out_summary << "time,C,mean_payoff,var_payoff\n";

    std::cout << std::left
              << std::setw(6) << "time"
              << std::setw(10) << "C ratio"
              << std::setw(15) << "mean"
              << std::setw(15) << "var"
              << std::endl;
}

void Simulation::run() {
    log_snapshot(0);

    for (int t = 0; t < cfg.steps;) {
        model->refresh(t, lattice);

        for (int i = 0; i < cfg.L * cfg.L; ++i){
            fermi_update(lattice, cfg.K);
        }
        t++;

        if (t % cfg.snapshot_interval == 0) {
            double c = log_snapshot(t);
            if (c < 1e-6 || c > 1.0 - 1e-6) {
                std::cout << "[Terminated early at t = " << t << "] Fixation reached.\n";
                break;
            }
        }
    }

    //Finally, take a snapshot at t = cfg.steps
    //model->refresh(cfg.steps, lattice);      // Optional, if r(t) matters at t = T
    //log_snapshot(cfg.steps);

    save_metadata(prefix, 0.0, "none");
    out_summary.close();
    std::cout << "Simulation complete." << std::endl;
}

double Simulation::log_snapshot(int t) {
    compute_global_payoffs(lattice);  // Run all PGGs for proper payoff collection

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