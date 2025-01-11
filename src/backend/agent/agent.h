#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <list>
#include <tuple>
#include <random>

#include "../stock/stock.h"
#include "../../util/queue.h"

class Agent {
public:
    Agent(double wealth);
    void analyze(const Stock& stock, std::random_device& rd);

private:
    double wealth_;
    double chance_;
    std::list<std::tuple <Stock*, unsigned int, Queue, double> > stocks_;
    // Stock | Owned Shares | Purchase History | ROI

    
};

#endif // AGENT_H