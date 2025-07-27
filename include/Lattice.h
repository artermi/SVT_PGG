#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include "Agent.h"
#include "Group.h"

class Lattice {
private:
    int L;
    std::vector<std::vector<Agent>> grid;
    std::vector<Group> groups;

public:
    Lattice(int size);
    void initialize_agents();
    void form_groups();
    std::vector<Group>& get_groups();
    Agent& get_agent(int x, int y);
    const Agent& get_agent(int x, int y) const;  //add this overload
    int size() const;
    Agent& random_neighbor(Agent& a);
    std::vector<int> get_flat_strategy_grid() const;
    double get_strategy_ratio() const;
};

#endif
