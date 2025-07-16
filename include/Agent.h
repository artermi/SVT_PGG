#ifndef AGENT_H
#define AGENT_H

class Agent {
public:
    int id, x, y;
    int strategy; // 1 = C, 0 = D
    double payoff;

    Agent(int id, int x, int y);
    void update_strategy(int new_strategy);
};

#endif