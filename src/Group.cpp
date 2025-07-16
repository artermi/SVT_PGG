#include "Group.h"

Group::Group() : r_value(1.0) {}

double Group::compute_payoff_for(Agent* a) const {
    int coop_count = 0;
    for (auto* member : members) coop_count += member->strategy;
    return r_value * (double) coop_count / (double) members.size() - (double)a->strategy;
}