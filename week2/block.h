#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "transaction.h"

struct Block {
    int id;
    int parent_id;
    int miner_id;
    double timestamp;
    std::vector<Transaction> transactions;
    int size_kb;

    // ✅ Default constructor
    Block() : id(-1), parent_id(-1), miner_id(-1), timestamp(0.0), size_kb(1) {}

    // ✅ Parameterized constructor
    Block(int id_, int parent, int miner, double time)
        : id(id_), parent_id(parent), miner_id(miner), timestamp(time), size_kb(1) {}

    void addTransaction(const Transaction& tx) {
        transactions.push_back(tx);
        size_kb++;
    }

    bool isFull() const {
        return size_kb >= 1024;
    }
};

#endif
