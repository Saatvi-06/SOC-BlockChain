#ifndef PEER_H
#define PEER_H

#include <set>
#include <vector>

struct Peer {
    int id;
    bool is_slow;
    bool is_low_cpu;
    int balance = 1000;
    std::set<int> seen_tx;
    std::set<int> seen_blocks;
    std::vector<int> neighbors;
    bool is_mining = false;
    int mining_tip_id = -1;

    Peer(int id_, bool slow, bool low_cpu)
        : id(id_), is_slow(slow), is_low_cpu(low_cpu) {}
};

#endif
