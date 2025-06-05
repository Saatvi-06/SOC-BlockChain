#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int MIN_PEERS = 50;
const int MAX_PEERS = 100;
const int MIN_DEGREE = 3;
const int MAX_DEGREE = 6;

class Network {
public:
    int n;
    vector<vector<int>> adj;

    Network() {
        srand(time(0));
        n = MIN_PEERS + rand() % (MAX_PEERS - MIN_PEERS + 1);
        adj.resize(n);
    }

    bool hasEdge(int u, int v) {
        for (int i = 0; i < adj[u].size(); ++i)
            if (adj[u][i] == v) return true;
        return false;
    }

    void addEdge(int u, int v) {
        if (!hasEdge(u, v) && u != v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    bool isConnected() {
        vector<bool> visited(n, false);
        dfs(0, visited);

        for (int i = 0; i < n; ++i)
            if (!visited[i]) return false;
        return true;
    }

    void dfs(int u, vector<bool>& visited) {
        visited[u] = true;
        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (!visited[v])
                dfs(v, visited);
        }
    }

    bool degreeValid() {
        for (int i = 0; i < n; ++i) {
            int d = adj[i].size();
            if (d < MIN_DEGREE || d > MAX_DEGREE)
                return false;
        }
        return true;
    }

    vector<int> getDegrees() {
        vector<int> degrees(n);
        for (int i = 0; i < n; ++i)
            degrees[i] = adj[i].size();
        return degrees;
    }

    void generateValidNetwork() {
        while (true) {
            // Reset graph
            for (int i = 0; i < n; ++i)
                adj[i].clear();

            // Random spanning tree to ensure connectivity
            for (int i = 1; i < n; ++i) {
                int j = rand() % i;
                addEdge(i, j);
            }

            // Add more edges to satisfy degree constraints
            for (int i = 0; i < n; ++i) {
                while (adj[i].size() < MIN_DEGREE) {
                    int j = rand() % n;
                    if (adj[j].size() < MAX_DEGREE)
                        addEdge(i, j);
                }
            }

            // Break if both conditions are met
            if (isConnected() && degreeValid())
                break;
        }
    }

    void writeToFile(const string& filename) {
        ofstream fout(filename.c_str());
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < adj[i].size(); ++j) {
                int v = adj[i][j];
                if (i < v) // avoid duplicates
                    fout << i << " " << v << "\n";
            }
        }
        fout.close();
    }
};

int main() {
    Network net;
    net.generateValidNetwork();
    net.writeToFile("edges.txt");

    cout << "Network with " << net.n << " peers generated successfully.\n";
    cout << "Edge list written to edges.txt\n";

    // Print degrees
    // vector<int> degrees = net.getDegrees();
    // cout << "Degrees of each peer:\n";
    // for (int i = 0; i < degrees.size(); ++i) {
    //     cout << "Peer " << i << ": " << degrees[i] << "\n";
    // }

    return 0;
}
