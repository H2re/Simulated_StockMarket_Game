#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "../include/simulation.h"
#include "../include/constants.h"
#include "../include/utils.h"

Simulation::Simulation()
    : logger_(),
      market_(logger_),
      running_(true),
      current_round_(0)
{
    // Create agents with random risk appetite
    for (int i = 0; i < NUM_AGENTS; ++i) {
        double riskAppetite = generate_random_real(0.0, 1.0);
        agents_.emplace_back(i, INITIAL_AGENT_WEALTH, riskAppetite);
    }

    // Seed the market with a few initial stocks
    for (int i = 0; i < 3; ++i) {
        int stock_id      = generate_random(1000, 9999);
        int stock_value   = generate_random(100, 500);
        int stock_vol     = generate_random(10, 30);
        double div_yield  = 0.01 * generate_random(0, 3);
        std::string sector = get_random_sector();
        std::string stock_name = get_random_stock_name();

        market_.addNewStock(stock_id, stock_name,
                            stock_value, stock_vol, 
                            div_yield, sector);
    }
}

void Simulation::start() {
    for (std::vector<Agent>::iterator it = agents_.begin(); 
         it != agents_.end(); ++it)
    {
        Agent &agent = *it;
        agent_threads_.emplace_back(&Simulation::agentBehavior, this, std::ref(agent));
    }

    market_thread_        = std::thread(&Simulation::marketWorker, this);
    stock_manager_thread_ = std::thread(&Simulation::stockManager, this);
    logger_thread_        = std::thread(&Simulation::loggerWorker, this);
    macro_event_thread_   = std::thread(&Simulation::macroEventWorker, this);

    std::this_thread::sleep_for(std::chrono::seconds(30));
    stop();
}

void Simulation::stop() {
    if (!running_) return;
    running_ = false;

    // Join agent threads
    for (std::vector<std::thread>::iterator it = agent_threads_.begin();
         it != agent_threads_.end(); ++it)
    {
        if (it->joinable()) {
            it->join();
        }
    }
    agent_threads_.clear();

    if (stock_manager_thread_.joinable()) stock_manager_thread_.join();
    if (market_thread_.joinable())        market_thread_.join();
    if (logger_thread_.joinable())        logger_thread_.join();
    if (macro_event_thread_.joinable())   macro_event_thread_.join();

    displayFinalResults();
}

void Simulation::marketWorker() {
    while (running_) {
        market_.adjustAllStocks(current_round_);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        current_round_++;
    }
}

void Simulation::stockManager() {
    while (running_) {
        {
            std::unordered_map<int, Stock> snapshot = market_.getStocksSnapshot();
            if (static_cast<int>(snapshot.size()) < MAX_STOCKS) {
                int stock_id       = generate_random(1000, 9999);
                int value          = generate_random(MIN_STOCK_VALUE, MAX_STOCK_VALUE);
                int vol            = generate_random(10, MAX_VOLATILITY);
                double dividend    = 0.01 * generate_random(0, 5);
                std::string sector = get_random_sector();
                std::string stock_name = get_random_stock_name();

                market_.addNewStock(stock_id, stock_name, value, vol, dividend, sector);
            } else {
                logger_.log("Stock Manager: Maximum stock limit reached.");
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void Simulation::loggerWorker() {
    while (running_) {
        logger_.printAll();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Simulation::macroEventWorker() {
    while (running_) {
        if (generate_random(0, 100) < MACRO_EVENT_PROB_PERCENT) {
            market_.applyMacroEvent();
        }
        if (generate_random(0, 100) < 5) {
            market_.applySectorRotation();
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void Simulation::agentBehavior(Agent &agent) {
    while (running_) {
        std::unordered_map<int, Stock> snapshot = market_.getStocksSnapshot();

        // SELLING LOGIC
        const std::unordered_map<int, OwnedStock> &ownedMap = agent.getOwnedStocks();
        for (std::unordered_map<int, OwnedStock>::const_iterator it = ownedMap.begin(); 
             it != ownedMap.end(); )
        {
            int stockId = it->first;
            const OwnedStock &ostock = it->second;

            if (snapshot.count(stockId)) {
                const Stock &stock = snapshot.at(stockId);
                double trend       = market_.calculateTrend(stock);
                int currentValue   = stock.getValue();

                double costBasis   = ostock.costBasis;
                int quantity       = ostock.quantity;

                double risk = agent.getRiskAppetite();
                double profitThresholdFactor   = 0.1 + 0.1 * risk;
                double stopLossThresholdFactor = 0.2 - 0.1 * risk;

                double profitThreshold   = costBasis * (1.0 + profitThresholdFactor);
                double stopLossThreshold = costBasis * (1.0 - stopLossThresholdFactor);

                bool shouldSell = false;
                if ((trend < 0.0) || (currentValue > profitThreshold) || 
                    (currentValue < stopLossThreshold))
                {
                    shouldSell = true;
                }

                if (shouldSell) {
                    double sellProceeds  = currentValue * quantity;
                    double totalCost     = costBasis   * quantity;
                    double profitOrLossD = sellProceeds - totalCost;
                    int    profitOrLoss  = static_cast<int>(profitOrLossD);

                    // Apply transaction fee (1% for example)
                    double fee = 0.01 * sellProceeds;
                    sellProceeds -= fee;

                    // Update agent wealth
                    agent.addWealth((int)sellProceeds);
                    market_.recordTransaction(stockId, current_round_);

                    // Immediately impact stock price (small decrement)
                    int decrement = generate_random(1, stock.getVolatility() / 8);
                    market_.applyInstantPriceChange(stockId, -decrement);

                    // Update biggest gain/loss if needed
                    agent.updateProfitLoss(profitOrLoss);

                    // Log transaction
                    logger_.log("Agent " + std::to_string(agent.getId()) +
                                (profitOrLoss >= 0 
                                    ? " sold stock " 
                                    : " stopped loss on stock ") +
                                stock.getName() +
                                " (Quantity=" + std::to_string(quantity) +
                                ", CostBasis=" + to_string_with_precision(costBasis, 2) +
                                ", Old Stock Price=" + std::to_string(currentValue) +
                                ", New Stock Price=" + 
                                std::to_string(market_.getStockValue(stockId)) +
                                ", Profit/Loss=" + std::to_string(profitOrLoss) +
                                ", Transaction Fee=" + to_string_with_precision(fee, 2) + ")");

                    // Erase from the agent's portfolio
                    std::unordered_map<int, OwnedStock> &ownedRef =
                        const_cast<std::unordered_map<int, OwnedStock>&>(agent.getOwnedStocks());
                    std::unordered_map<int, OwnedStock>::const_iterator to_erase = it;
                    ++it;
                    ownedRef.erase(to_erase->first);
                    continue;
                }
            }
            ++it;
        }

        // BUYING LOGIC
        for (std::unordered_map<int, Stock>::const_iterator kv = snapshot.begin(); 
             kv != snapshot.end(); ++kv) 
        {
            const Stock &stock = kv->second;
            int stockId        = stock.getId();
            int currentValue   = stock.getValue();

            // 50% probability to consider buying
            if (generate_random(0, 100) < 50) {
                // We'll buy only if the price < half the agent's wealth
                if (currentValue < agent.getWealth() / 2 &&
                    agent.getWealth() > currentValue)
                {
                    int quantity = generate_random(1, 5);
                    int totalCost = quantity * currentValue;

                    // If we can't afford that many shares, reduce quantity
                    while (quantity > 0 && (totalCost > agent.getWealth())) {
                        quantity--;
                        totalCost = quantity * currentValue;
                    }

                    if (quantity > 0 && totalCost <= agent.getWealth()) {
                        int old_value = currentValue;

                        // Calculate transaction fee (1%)
                        double fee = 0.01 * totalCost;
                        int finalCost = totalCost + (int)fee;

                        agent.addWealth(-finalCost);
                        agent.buyStock(stockId, quantity, (double)currentValue);

                        market_.recordTransaction(stockId, current_round_);

                        // Immediately impact stock price (small increment)
                        int increment = generate_random(1, stock.getVolatility() / 8);
                        market_.applyInstantPriceChange(stockId, increment);

                        logger_.log("Agent " + std::to_string(agent.getId()) +
                                    " bought stock " + 
                                    stock.getName() +
                                    " (Quantity=" + std::to_string(quantity) +
                                    ", Price Per Share=" + std::to_string(old_value) +
                                    ", Transaction Fee=" + to_string_with_precision(fee, 2) +
                                    ", New Stock Price=" +
                                    std::to_string(market_.getStockValue(stockId)) + ")");
                    }
                }
            }
        }
        // Simulate agent thinking time
        std::this_thread::sleep_for(std::chrono::milliseconds(generate_random(100, 500)));
    }
}

void Simulation::displayFinalResults() {
    logger_.printAll();
    // Final Stock Details
    std::cout << "\n--- Final Stock Details ---\n";
    std::unordered_map<int, Stock> snapshot = market_.getStocksSnapshot();
    for (std::unordered_map<int, Stock>::const_iterator p = snapshot.begin();
         p != snapshot.end(); ++p)
    {
        const Stock &stock = p->second;
        std::cout << "Stock " << stock.getId() 
                  << " (" << stock.getName() << ", Sector: " << stock.getSector() << ")\n"
                  << "  Final Value: " << stock.getValue() << "\n"
                  << "  Transactions: " << stock.getTransactionCount() << "\n"
                  << "  Dividend Yield: " << std::fixed << std::setprecision(2)
                  << (stock.getDividendYield() * 100) << "%\n"
                  << "  History: ";
        const std::deque<int> &hist = stock.getHistory();
        for (std::deque<int>::const_iterator hv = hist.begin(); hv != hist.end(); ++hv) {
            std::cout << *hv << " ";
        }
        std::cout << "\n";
    }
    std::cout << "----------------------------\n";

    // Agent Portfolios
    for (std::vector<Agent>::iterator it = agents_.begin(); 
         it != agents_.end(); ++it)
    {
        Agent &agent = *it;
        int portfolio_value = 0;
        std::cout << "Agent " << agent.getId() << ":\n"
                  << "  Wealth: " << agent.getWealth() << "\n"
                  << "  Risk Appetite: " << std::fixed << std::setprecision(2)
                  << agent.getRiskAppetite() << "\n"
                  << "  Biggest Gain (Single Transaction): " << agent.getBiggestGain() << "\n"
                  << "  Biggest Loss (Single Transaction): " << agent.getBiggestLoss() << "\n"
                  << "  Portfolio:\n";

        const std::unordered_map<int, OwnedStock> &ownedMap = agent.getOwnedStocks();
        for (std::unordered_map<int, OwnedStock>::const_iterator owned_stock = ownedMap.begin();
             owned_stock != ownedMap.end(); ++owned_stock)
        {
            int stock_id = owned_stock->first;
            const OwnedStock &info = owned_stock->second;

            int current_value = snapshot.count(stock_id)
                                ? snapshot.at(stock_id).getValue()
                                : 0;

            int total_current_value = current_value * info.quantity;
            portfolio_value += total_current_value;

            std::string stock_name = "(Unknown)";
            if (snapshot.count(stock_id)) {
                stock_name = snapshot.at(stock_id).getName();
            }

            std::cout << "    Stock " << stock_id
                      << " (" << stock_name << ")"
                      << " | Quantity: " << info.quantity
                      << " | CostBasis: " << std::fixed << std::setprecision(2)
                      << info.costBasis
                      << " | Current Value/Share: " << current_value
                      << " | Total Value: " << total_current_value << "\n";
        }

        int total_wealth = agent.getWealth() + portfolio_value;
        std::cout << "  Total Wealth (Including Portfolio): " << total_wealth << "\n\n";
    }

    std::cout << "Simulation ended.\n";
}