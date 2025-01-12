#ifndef __AGENT_H__
#define __AGENT_H__

#include <unordered_map>
#include <string>

struct OwnedStock {
    int quantity;
    double costBasis;
};

class Agent {
public:
    Agent(int id, int initialWealth, double riskAppetite);

    int getId() const { return id_; }
    int getWealth() const { return wealth_; }
    double getRiskAppetite() const { return risk_appetite_; }
    int getBiggestGain() const { return biggestGain_; }
    int getBiggestLoss() const { return biggestLoss_; }
    bool ownsStock(int stockId) const { return owned_stocks_.count(stockId) > 0; }

    void payDividend(int amount) { wealth_ += amount; }
    void addWealth(int amount) { wealth_ += amount; }
    void updateProfitLoss(int profitOrLoss);
    void sellAllShares(int stockId) { owned_stocks_.erase(stockId); };
    void buyStock(int stockId, int quantity, double pricePerShare);
    const std::unordered_map<int, OwnedStock> &getOwnedStocks() const { return owned_stocks_; }

private:
    int id_;
    int wealth_;
    double risk_appetite_;
    int biggestGain_;
    int biggestLoss_;

    // Map: stock ID -> OwnedStock
    std::unordered_map<int, OwnedStock> owned_stocks_;
};

#endif /* __AGENT_H__ */