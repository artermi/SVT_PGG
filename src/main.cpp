#include "Config.h"
#include "Simulation.h"
#include <cstdlib>   // for getenv
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " r0 sigma model [tau]\n";
        std::cerr << "  model: A (spatial) or B (temporal)\n";
        std::cerr << "  tau: required if model = B\n";
        return 1;
    }

    Config config;
    config.r0 = std::stod(argv[1]);
    config.sigma = std::stod(argv[2]);

    std::string model_flag = argv[3];
    if (model_flag == "A") {
        config.model_type = ModelType::SPATIAL;
    } else if (model_flag == "B") {
        if (argc < 5) {
            std::cerr << "Model B requires tau parameter\n";
            return 1;
        }
        config.model_type = ModelType::TEMPORAL;
        config.tau = std::stoi(argv[4]);
    } else {
        std::cerr << "Invalid model type: " << model_flag << " (expected A or B)\n";
        return 1;
    }


    // --- New: seed handling ---
    unsigned int base_seed = 42; // fallback if not provided
    if (const char* env_p = std::getenv("PG_SEED")) {
        base_seed = std::stoul(env_p);
    }

    std::mt19937 rng_main(base_seed);         // for lattice init, shuffle, neighbour choice
    std::mt19937 rng_r(base_seed + 12345);    // for r draws only

    // You now have two RNGs. There are two ways to use them:
    // (a) Pass them into Simulation (preferred).
    // (b) Make them global/singletons if Simulation expects to construct its own.

    Simulation sim(config, rng_main, rng_r);  // <-- you’ll need to add overload to Simulation
    sim.run();
    return 0;
}