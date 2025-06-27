#ifndef EVENT_H
#define EVENT_H

enum class EventType {
    GENERATE_TX,
    RECEIVE_TX,
    MINE_BLOCK,
    RECEIVE_BLOCK
};

struct Event {
    double time;
    EventType type;
    int peer_id;
    int target_id;
    int tx_id;
    int block_id;

    bool operator<(const Event& other) const {
        return time > other.time;
    }
};

#endif
