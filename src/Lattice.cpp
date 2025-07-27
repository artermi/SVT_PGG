#include "Lattice.h"
#include <random>
#include <iostream>

Lattice::Lattice(int size) : L(size) {
    initialize_agents();
    form_groups();
}

void Lattice::initialize_agents() {
    std::mt19937 rng(std::random_device{}());  // or use fixed seed
    std::uniform_int_distribution<int> binary(0, 1);

    grid.resize(L);
    int id = 0;
    for (int x = 0; x < L; ++x) {
        grid[x].reserve(L);
        for (int y = 0; y < L; ++y) {
            int strategy = binary(rng);  // 0 or 1
            grid[x].emplace_back(id++, x, y, strategy);
        }
    }

}

void Lattice::form_groups() {
    groups.reserve(L * L);
    for (int x = 0; x < L; ++x) {
        for (int y = 0; y < L; ++y) {
            Group g;
            std::vector<Agent*> members = {
                &grid[x][y],
                &grid[(x+1)%L][y],
                &grid[(x+L-1)%L][y],
                &grid[x][(y+1)%L],
                &grid[x][(y+L-1)%L]
            };
            g.members = members;

            // Insert group into list exactly once
            groups.push_back(std::move(g));
            Group* gptr = &groups.back();

            // Assign this group to each agent's group list
            for (Agent* a : members)
                a->groups.push_back(gptr);
        }
    }
}
std::vector<Group>& Lattice::get_groups() { return groups; }
Agent& Lattice::get_agent(int x, int y) { return grid[x][y]; }
const Agent& Lattice::get_agent(int x, int y) const {
    return grid[x][y];
}
int Lattice::size() const { return L; }

Agent& Lattice::random_neighbor(Agent& a) {
    static std::mt19937 rng(123);
    static std::uniform_int_distribution<int> dist(0, 3);
    int dir = dist(rng);
    int nx = a.x, ny = a.y;
    if (dir == 0) nx = (a.x + 1) % L;
    if (dir == 1) nx = (a.x + L - 1) % L;
    if (dir == 2) ny = (a.y + 1) % L;
    if (dir == 3) ny = (a.y + L - 1) % L;
    return grid[nx][ny];
}

double Lattice::get_strategy_ratio() const {
    int total = 0, cooperators = 0;
    for (const auto& row : grid) {
        for (const auto& agent : row) {
            ++total;
            if (agent.strategy == 1) ++cooperators;
        }
    }
    return static_cast<double>(cooperators) / total;
}