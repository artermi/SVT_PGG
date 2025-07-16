#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <cmath>
#include <string>
#include <numeric>
#include <nlohmann/json.hpp>
#include "Lattice.h"
#include "ModelA.h"
#include "ModelB.h"

using json = nlohmann::json;

void compute_payoffs(Lattice& lattice) {
    for (int x = 0; x < lattice.size(); ++x) {
        for (int y = 0; y < lattice.size(); ++y) {
            Agent& a = lattice.get_agent(x, y);
            a.payoff = 0.0;
        }
    }
    for (auto& g : lattice.get_groups()) {
        for (auto* a : g.members) {
            a->payoff += g.compute_payoff_for(a);
        }
    }
}

double mean_payoff(const Lattice& lattice) {
    double sum = 0.0;
    int count = 0;
    for (int x = 0; x < lattice.size(); ++x)
        for (int y = 0; y < lattice.size(); ++y) {
            sum += lattice.get_agent(x, y).payoff;
            ++count;
        }
    return sum / count;
}

double var_payoff(const Lattice& lattice, double mean) {
    double var = 0.0;
    int count = 0;
    for (int x = 0; x < lattice.size(); ++x)
        for (int y = 0; y < lattice.size(); ++y) {
            double diff = lattice.get_agent(x, y).payoff - mean;
            var += diff * diff;
            ++count;
        }
    return var / count;
}

void fermi_update(Lattice& lattice, double K) {
    std::vector<Agent*> agents;
    for (int x = 0; x < lattice.size(); ++x)
        for (int y = 0; y < lattice.size(); ++y)
            agents.push_back(&lattice.get_agent(x, y));

    std::shuffle(agents.begin(), agents.end(), std::mt19937{std::random_device{}()});
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    for (Agent* a : agents) {
        Agent& b = lattice.random_neighbor(*a);
        double W = 1.0 / (1.0 + std::exp((a->payoff - b.payoff) / K));
        if (dist(rng) < W) a->update_strategy(b.strategy);
    }
}

void save_summary(int t, double c, double mean, double var, std::ofstream& out_summary) {
    out_summary << t << "," << c << "," << mean << "," << var << "\n";
}

void save_metadata(const std::string& prefix, double xi, const std::string& fixation) {
    json j;
    j["correlation_length"] = xi;
    j["fixation"] = fixation;
    std::ofstream out("data/" + prefix + "_metadata.json");
    out << j.dump(2);
}

int main() {
    int L = 100;
    double r0 = 2.5, sigma = 0.5, K = 0.1;
    std::string ftype = "uniform";
    int steps = 1000;
    int snapshot_interval = 100;
    int tau = 10; // only used in ModelB
    bool use_model_b = false; // toggle between ModelA and ModelB

    std::string prefix = "r" + std::to_string(int(r0 * 10)) +
                         "v" + std::to_string(int(sigma * 100)) +
                         "_" + ftype +
                         "_L" + std::to_string(L);

    Lattice lattice(L);
    std::unique_ptr<RandomnessModel> model;

    if (use_model_b) {
        model = std::make_unique<ModelB>(r0, sigma, L, tau);
        prefix += "_Btau" + std::to_string(tau);
    } else {
        model = std::make_unique<ModelA>(r0, sigma);
        prefix += "_A";
    }

    model->initialize(lattice);
    std::ofstream out_summary("data/" + prefix + "_summary.csv");
    out_summary << "time,C,mean_payoff,var_payoff\n";

    for (int t = 0; t <= steps; ++t) {
        model->refresh(t, lattice);
        compute_payoffs(lattice);
        fermi_update(lattice, K);

        if (t % snapshot_interval == 0) {
            auto [d, c] = lattice.get_strategy_ratio();
            double mean = mean_payoff(lattice);
            double var = var_payoff(lattice, mean);
            save_summary(t, c, mean, var, out_summary);
        }
    }

    double correlation_length = 0.0;
    std::string fixation_state = "none";
    save_metadata(prefix, correlation_length, fixation_state);

    out_summary.close();
    std::cout << "Simulation complete with OO model support." << std::endl;
    return 0;
}