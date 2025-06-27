#ifndef NETWORK_H
#define NETWORK_H

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

class Network {
public:
    std::vector<std::vector<int>> adj;

    void loadFromFile(const std::string& filename) {
        std::ifstream fin(filename);
        int u, v, maxn = -1;
        while (fin >> u >> v) {
            maxn = std::max({maxn, u, v});
            adj.resize(maxn + 1);
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        fin.close();
    }
};

#endif
