#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include "Agent.h"

class Group {
public:
    std::vector<Agent*> members;
    double r_value;

    Group();
    double compute_payoff_for(Agent* a) const;
};

#endif