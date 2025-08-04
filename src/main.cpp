#include "Config.h"
#include "Simulation.h"
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

    Simulation sim(config);
    sim.run();
    return 0;
}