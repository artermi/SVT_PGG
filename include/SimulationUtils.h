#pragma once

#include "Lattice.h"
#include "Agent.h"
#include <string>  // add this line

double compute_local_payoff(Agent& a);
void fermi_update(Lattice& lattice, double K);
double mean_payoff(const Lattice& lattice);
double var_payoff(const Lattice& lattice, double mean);
void save_metadata(const std::string& prefix, double xi, const std::string& fixation);
