#ifndef BLOCKCHAIN_TREE_H
#define BLOCKCHAIN_TREE_H

#include <unordered_map>
#include "block.h"

class BlockchainTree {
public:
    std::unordered_map<int, Block> blocks;
    std::unordered_map<int, int> depth;
    int tip = 0;

    BlockchainTree() {
        Block genesis(0, -1, -1, 0.0);
        blocks[0] = genesis;
        depth[0] = 0;
    }

    void addBlock(const Block& b) {
        if (blocks.count(b.id)) return;
        blocks[b.id] = b;
        depth[b.id] = depth[b.parent_id] + 1;
        if (depth[b.id] > depth[tip]) tip = b.id;
    }

    int getLongestChainTip() const {
        return tip;
    }

    bool contains(int id) const {
        return blocks.count(id);
    }

    bool validate(const Block& b) const {
        return blocks.count(b.parent_id);
    }
};

#endif
