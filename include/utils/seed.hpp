#pragma once
#include <cstdint>
#include <cstdlib>
#include <string>

inline uint64_t read_seed_from_env(const char* var = "PG_SEED") {
    if (const char* s = std::getenv(var)) {
        try { return static_cast<uint64_t>(std::stoull(std::string{s})); }
        catch (...) {}
    }
    return 0ull; // default if not set / malformed
}