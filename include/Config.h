#pragma once
#include <string>

enum class ModelType { SPATIAL, TEMPORAL };

struct Config {
    int L = 100;
    double r0 = 2.5;
    double sigma = 0.5;
    double K = 0.1;
    int steps = 5000;
    int snapshot_interval = 100;
    int tau = 10;
    ModelType model_type = ModelType::SPATIAL;  // clear and extensible
    std::string ftype = "uniform";
};