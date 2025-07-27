// Config.h

#pragma once
#include <string>

struct Config {
    int L = 100;
    double r0 = 2.5;
    double sigma = 0.5;
    double K = 0.1;
    int steps = 100;
    int snapshot_interval = 10;
    int tau = 10;
    bool use_model_b = false;
    std::string ftype = "uniform";
};