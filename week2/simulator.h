#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <cmath>
#include <iostream>
#include "event.h"
#include "peer.h"
#include "block.h"
#include "transaction.h"
#include "blockchain_tree.h"
#include "latency.h"
#include "network.h"

class Simulator {
public:
    double global_time = 0;
    int next_tx_id = 1, next_blk_id = 1;
    double Ttx, I;

    std::priority_queue<Event> pq;
    std::vector<Peer> peers;
    std::vector<BlockchainTree> chains;
    Network network;
    LatencyManager latency;

    std::map<int, Transaction> all_txns;
    std::map<int, Block> all_blocks;

    Simulator(int n, double Ttx_, double I_, double z0, double z1)
        : Ttx(Ttx_), I(I_) {
        srand(time(nullptr));
        for (int i = 0; i < n; i++) {
            bool slow = rand()%100 < z0;
            bool low = rand()%100 < z1;
            peers.emplace_back(i, slow, low);
            chains.emplace_back();
        }

        network.loadFromFile("edges.txt");
        for (int i = 0; i < peers.size(); i++)
            peers[i].neighbors = network.adj[i];

        for (int i = 0; i < peers.size(); i++) {
            scheduleTx(i, expSample(Ttx));
            scheduleMine(i);
        }
    }

    double expSample(double mean) {
        return -mean * log((double)rand()/RAND_MAX);
    }

    void scheduleTx(int id, double dt) {
        pq.push({global_time + dt, EventType::GENERATE_TX, id, -1, -1, -1});
    }

    void scheduleMine(int id) {
        double h = peers[id].is_low_cpu ? 1 : 10;
        double dt = expSample(I/h);
        peers[id].is_mining = true;
        peers[id].mining_tip_id = chains[id].getLongestChainTip();
        pq.push({global_time + dt, EventType::MINE_BLOCK, id, -1, -1, -1});
    }

    void run(double maxt) {
        while (!pq.empty()) {
            Event e = pq.top(); pq.pop();
            global_time = e.time;
            if (global_time > maxt) break;

            switch (e.type) {
            case EventType::GENERATE_TX: genTx(e.peer_id); break;
            case EventType::RECEIVE_TX: recvTx(e.peer_id, e.target_id, e.tx_id); break;
            case EventType::MINE_BLOCK: mineBlk(e.peer_id); break;
            case EventType::RECEIVE_BLOCK: recvBlk(e.peer_id, e.block_id, e.target_id); break;
            }
        }
        writeChains();
    }

    void genTx(int id) {
        int to = rand() % peers.size();
        while (to==id) to = rand() % peers.size();
        int amt = rand()%10 + 1;
        if (peers[id].balance < amt) { scheduleTx(id,expSample(Ttx)); return; }

        Transaction tx{next_tx_id++,id,to,amt};
        peers[id].balance -= amt;
        all_txns[tx.id] = tx;
        peers[id].seen_tx.insert(tx.id);
        broadcastTx(id, id, tx.id);

        scheduleTx(id, expSample(Ttx));
    }

    void broadcastTx(int src, int from, int txid) {
        for (int v : peers[src].neighbors) {
            if (v == from) continue;
            double d = latency.computeLatency(src,v,8,!peers[src].is_slow,!peers[v].is_slow);
            pq.push({global_time + d, EventType::RECEIVE_TX, v, src, txid, -1});
        }
    }

    void recvTx(int id, int from, int txid) {
        if (peers[id].seen_tx.count(txid)) return;
        peers[id].seen_tx.insert(txid);
        broadcastTx(id, from, txid);
    }

    void mineBlk(int id) {
        if (!peers[id].is_mining) return;
        int tip = chains[id].getLongestChainTip();
        if (tip != peers[id].mining_tip_id) return;

        Block blk(next_blk_id++, tip, id, global_time);
        Transaction cb{-blk.id,-1,id,50};
        blk.addTransaction(cb);
        peers[id].balance += 50;

        for (int txid : peers[id].seen_tx) {
            if (blk.isFull()) break;
            auto &tx = all_txns[txid];
            if (peers[id].balance>=tx.amount) {
                peers[id].balance -= tx.amount;
                blk.addTransaction(tx);
            }
        }

        all_blocks[blk.id] = blk;
        chains[id].addBlock(blk);
        broadcastBlk(id, blk.id);

        peers[id].is_mining = false;
        scheduleMine(id);
    }

    void broadcastBlk(int src, int blkid) {
        auto &blk = all_blocks[blkid];
        for (int v : peers[src].neighbors) {
            double d = latency.computeLatency(src,v,blk.size_kb*8,!peers[src].is_slow,!peers[v].is_slow);
            pq.push({global_time + d, EventType::RECEIVE_BLOCK, v, src, -1, blkid});
        }
    }

    void recvBlk(int id, int from, int blkid) {
        if (peers[id].seen_blocks.count(blkid)) return;
        peers[id].seen_blocks.insert(blkid);
        auto &blk = all_blocks[blkid];
        if (!chains[id].validate(blk)) return;
        chains[id].addBlock(blk);
        broadcastBlk(id, blkid);

        if (peers[id].is_mining && peers[id].mining_tip_id != chains[id].getLongestChainTip()) {
            peers[id].is_mining = false;
            scheduleMine(id);
        }
    }

    void writeChains() {
        for (int i = 0; i < peers.size(); i++) {
            std::ofstream f("peer_"+std::to_string(i)+"_chain.txt");
            for (auto &p : chains[i].blocks) {
                auto &blk = p.second;
                f << "Block "<<blk.id<<" mined by Peer "<<blk.miner_id
                  <<", parent: "<<blk.parent_id
                  <<", time: "<<blk.timestamp
                  <<", txs: "<<blk.transactions.size()<<"\n";
            }
        }
    }
};

#endif
