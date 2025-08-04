#pragma once

#include "Lattice.h"
#include "Agent.h"
#include <string>  // add this line

double compute_local_payoff(Agent& a);
void fermi_update(Lattice& lattice, double K);
void compute_global_payoffs(Lattice& lattice);
double mean_payoff(const Lattice& lattice);
double var_payoff(const Lattice& lattice, double mean);
void save_metadata(const std::string& prefix, double xi, const std::string& fixation);
std::string format_prefix(double r0, double sigma, const std::string& ftype,
                          int L, bool is_model_b, int tau);
