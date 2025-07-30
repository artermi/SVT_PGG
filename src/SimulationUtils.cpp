#include "SimulationUtils.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <random>
#include <cmath>
#include <string>
#include <algorithm>

using json = nlohmann::json;

// Compute payoff for one agent by scanning all groups that include it
double compute_local_payoff(Agent& a) {
    double total = 0.0;
    for (Group* g : a.groups) {
        total += g->compute_payoff_for(&a);
    }
    return total;
}

// Compute global payoffs for all agents by running each group once
void compute_global_payoffs(Lattice& lattice) {
    // Reset all agent payoffs
    for (int x = 0; x < lattice.size(); ++x) {
        for (int y = 0; y < lattice.size(); ++y) {
            lattice.get_agent(x, y).payoff = 0.0;
        }
    }
    // Accumulate payoff from each group
    for (Group& g : lattice.get_groups()) {
        for (Agent* a : g.members) {
            a->payoff += g.compute_payoff_for(a);
        }
    }
}


void fermi_update(Lattice& lattice, double K) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> coord(0, lattice.size() - 1);
    std::uniform_real_distribution<> dist(0.0, 1.0);

    int x = coord(rng);
    int y = coord(rng);
    Agent& a = lattice.get_agent(x, y);
    Agent& b = lattice.random_neighbor(a);
    if (a.strategy == b.strategy)
        return;

    double Pi = compute_local_payoff(a);
    double Pj = compute_local_payoff(b);

    double W = 1.0 / (1.0 + std::exp((Pi - Pj) / K));
    if (dist(rng) < W)
        a.update_strategy(b.strategy);
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

void save_metadata(const std::string& prefix, double xi, const std::string& fixation) {
    json j;
    j["correlation_length"] = xi;
    j["fixation"] = fixation;

    std::ofstream out("data/" + prefix + "_metadata.json");
    out << j.dump(2);
}
