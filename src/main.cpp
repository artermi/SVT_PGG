#include "Config.h"
#include "Simulation.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " r0 sigma\n";
        return 1;
    }

    Config config;
    config.r0 = std::stod(argv[1]);
    config.sigma = std::stod(argv[2]);

    Simulation sim(config);
    sim.run();
    return 0;
}