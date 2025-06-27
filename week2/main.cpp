#include <iostream>
#include "simulator.h"

int main(int argc, char* argv[]) {
    if (argc < 6) {
        std::cout << "Usage: ./simulator <n> <z0_slow%> <z1_lowCPU%> <Ttx_mean> <I_mean>\n";
        return 1;
    }
    int n = atoi(argv[1]);
    double z0 = atof(argv[2]);
    double z1 = atof(argv[3]);
    double Ttx = atof(argv[4]);
    double I = atof(argv[5]);

    Simulator sim(n, Ttx, I, z0, z1);
    sim.run(10000.0);
    std::cout<<"Simulation done.\n";
    return 0;
}
