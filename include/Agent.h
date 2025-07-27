#pragma once


#ifndef AGENT_H
#define AGENT_H
#include <vector>
class Group;  // ✅ this tells the compiler that Group exists


class Agent {
public:
    int id, x, y;
    int strategy; // 1 = C, 0 = D
    double payoff;
    std::vector<Group*> groups;


    Agent(int id, int x, int y, int strategy);
    void update_strategy(int new_strategy);
};

#endif