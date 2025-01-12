#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <atomic>
#include <vector>
#include <thread>

#include "transaction_logger.h"
#include "stock_market.h"
#include "agent.h"

class Simulation {
public:
    Simulation();
    ~Simulation() { stop(); }
    void start();

private:
    void stop();
    void marketWorker();
    void stockManager();
    void loggerWorker();
    void macroEventWorker();
    void agentBehavior(Agent &agent);
    void displayFinalResults();

private:
    std::atomic<bool> running_;
    int current_round_;

    TransactionLogger logger_;
    StockMarket market_;
    std::vector<Agent> agents_;

    /* Threads */
    std::vector<std::thread> agent_threads_;
    std::thread stock_manager_thread_;
    std::thread market_thread_;
    std::thread logger_thread_;
    std::thread macro_event_thread_;
};

#endif /* __SIMULATION_H__ */