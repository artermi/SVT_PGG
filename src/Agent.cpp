#include "Agent.h"
#include <random>


Agent::Agent(int id, int x, int y, int strategy)
    : id(id), x(x), y(y), strategy(strategy), payoff(0.0) {}

void Agent::update_strategy(int new_strategy) {
    strategy = new_strategy;
}