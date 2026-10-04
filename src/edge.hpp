#ifndef EDGE
#define EDGE

class Edge {
    bool state = false;
    bool lastState = false;

public:

    void step(bool newState) {
        lastState = state;
        state = newState;
    }
    bool rising() {
        return state && (! lastState);
    }
    bool falling() {
        return (!state) && lastState;
    }
};

#endif