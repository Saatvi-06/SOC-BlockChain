#ifndef LATENCY_H
#define LATENCY_H

#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <cmath>
#include <string>

class LatencyManager {
    std::unordered_map<std::string, double> rho;

public:
    LatencyManager() {
        srand(time(nullptr));
    }

    double getPropagationDelay(int i, int j) {
        std::string key = std::to_string(std::min(i,j)) + "-" + std::to_string(std::max(i,j));
        if (!rho.count(key)) {
            rho[key] = 10 + rand() % 491;
        }
        return rho[key];
    }

    double computeLatency(int i, int j, int msg_size_kbits, bool fast_i, bool fast_j) {
        double r = getPropagationDelay(i, j);
        double c = (fast_i && fast_j) ? 100000.0 : 5000.0;
        double mean_q = 96.0 / c;
        double q = -mean_q * log((double)rand() / RAND_MAX);
        return r + (msg_size_kbits / c) + q;
    }
};

#endif
