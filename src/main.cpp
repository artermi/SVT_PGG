#include "Config.h"
#include "Simulation.h"

int main() {
    Config config;
    Simulation sim(config);
    sim.run();
    return 0;
}