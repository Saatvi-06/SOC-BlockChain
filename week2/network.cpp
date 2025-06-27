#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;

int main() {
    srand(time(NULL));
    // int n = rand() % 51 + 50; // 50 to 100 peers
    int n=90;
    int minDeg = 3, maxDeg = 6;
    vector<set<int>> adj(n);

    // Ensure connectivity using random spanning tree
    vector<int> perm(n);
    for (int i = 0; i < n; i++) perm[i] = i;
    random_shuffle(perm.begin(), perm.end());
    for (int i = 1; i < n; i++) {
        int u = perm[i], v = perm[rand() % i];
        adj[u].insert(v);
        adj[v].insert(u);
    }

    // Add extra edges to meet degree requirements
    for (int u = 0; u < n; u++) {
        while (adj[u].size() < minDeg) {
            int v = rand() % n;
            if (v != u && adj[u].count(v) == 0) {
                adj[u].insert(v);
                adj[v].insert(u);
            }
        }
    }

    // Trim any excess edges randomly to enforce maxDeg
    for (int u = 0; u < n; u++) {
        while (adj[u].size() > maxDeg) {
            int idx = rand() % adj[u].size();
            auto it = adj[u].begin();
            advance(it, idx);
            int v = *it;
            adj[u].erase(v);
            adj[v].erase(u);
        }
    }

    // Write edges to file
    ofstream fout("edges.txt");
    for (int u = 0; u < n; u++)
        for (int v : adj[u])
            if (u < v)
                fout << u << " " << v << "\n";
    fout.close();

    cout << "Generated network with " << n << " peers, edges.txt created.\n";
    return 0;
}
