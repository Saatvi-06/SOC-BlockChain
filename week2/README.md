# Blockchain Arena: Simulating Mining Wars and Network Attacks

## Week 2:  
### Assignment: Simulation of a P2P Cryptocurrency Network

---

##  Objective

This assignment builds upon the Week 1 P2P network by implementing a **discrete-event simulator** that models transaction generation, PoW mining, latency-aware message propagation, and blockchain formation across a network of heterogeneous peers.

---

## Implementation Details

###  Language: C++ (Primary Simulation Engine)

#### 1. `network.cpp`
- **Generates** a random, connected, undirected P2P network.
- **Peers**: Fixed at 90.
- **Degree constraints**: Each peer has between 3 and 6 neighbors.
- **Ensures**:
  - No self-loops or duplicate edges.
  - Overall graph connectivity.
- **Output**: Edge list saved to `edges.txt`.

#### 2. Core Simulator (`simulator.h` and related headers)
- **Event Queue**: Manages and processes events like `GENERATE_TX`, `RECEIVE_TX`, `MINE_BLOCK`, and `RECEIVE_BLOCK`.
- **Peers**:
  - Each peer has flags indicating if it's slow (low bandwidth) or low-CPU (low hash power).
  - Maintains its own blockchain tree (`BlockchainTree`) and state.
- **Transactions**:
  - Generated with exponential interarrival time (`Ttx`).
  - Forwarded only once to prevent cycles.
- **Blocks**:
  - Formed by miners using PoW simulation.
  - Includes a coinbase transaction (50 coins).
  - Size-limited to 1MB (1024 KB).
- **Network Latency**:
  - Modeled using propagation delay, bandwidth constraints, and exponential queuing delay.
- **Output**:
  - Each peer writes its blockchain state to `peer_<id>_chain.txt`.

---

##  How to Run

### Step 1: Generate the P2P Network

```bash
g++ network.cpp -o network
./network
```
This creates edges.txt, a peer connection file.
### Step 2: Build and Run the Simulator

```bash
g++ -std=c++11 -o simulator main.cpp
./simulator 90 <z0_slow%> <z1_lowCPU%> <Ttx_mean> <I_mean>
```
Where:

90: Fixed number of peers (can change it in the network.cpp) 

z0: % slow peers (e.g., 30)

z1: % low CPU peers (e.g., 50)

Ttx_mean: mean interarrival time of transactions (e.g., 30 seconds)

I_mean: mean block mining interval (e.g., 600 seconds)

## Output Files
edges.txt: P2P network graph (edge list)

peer_<id>_chain.txt: Each peer’s blockchain,
 showing: Block ID, Parent ID, Miner ID, Timestamp and Number of transactions in the format:
 "Block <Block ID> mined by Peer <Miner ID> parent: < Parent ID>, time: <Timestamp>, txs: <Number of transactions>"

 ##  Output Files

- `edges.txt`:  
  Contains the generated **P2P network graph** in the form of an undirected edge list.

- `peer_<id>_chain.txt`:  
  For each peer, this file records the peer’s **local blockchain view**, listing each block in the following format:
  ``` bash 
   Block <Block ID> mined by Peer <Miner ID>, parent: <Parent ID>, time: <Timestamp>, txs: <Number of transactions>```

