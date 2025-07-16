#include "Agent.h"

Agent::Agent(int id, int x, int y) : id(id), x(x), y(y), strategy(1), payoff(0.0) {}

void Agent::update_strategy(int new_strategy) {
    strategy = new_strategy;
}