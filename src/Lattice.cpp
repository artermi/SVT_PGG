#include "Lattice.h"
#include <random>

Lattice::Lattice(int size) : L(size) {
    initialize_agents();
    form_groups();
}

void Lattice::initialize_agents() {
    grid.resize(L, std::vector<Agent>(L));
    int id = 0;
    for (int x = 0; x < L; ++x) {
        for (int y = 0; y < L; ++y) {
            grid[x][y] = Agent(id++, x, y);
        }
    }
}

void Lattice::form_groups() {
    groups.reserve(L * L);
    for (int x = 0; x < L; ++x) {
        for (int y = 0; y < L; ++y) {
            Group g;
            g.members.push_back(&grid[x][y]);
            g.members.push_back(&grid[(x+1)%L][y]);
            g.members.push_back(&grid[(x+L-1)%L][y]);
            g.members.push_back(&grid[x][(y+1)%L]);
            g.members.push_back(&grid[x][(y+L-1)%L]);
            groups.push_back(g);
        }
    }
}

std::vector<Group>& Lattice::get_groups() { return groups; }
Agent& Lattice::get_agent(int x, int y) { return grid[x][y]; }
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