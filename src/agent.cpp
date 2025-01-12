#include "../include/agent.h"

Agent::Agent(int id, int initialWealth, double riskAppetite) : 
    id_(id),
    wealth_(initialWealth),
    risk_appetite_(riskAppetite),
    biggestGain_(0),
    biggestLoss_(0) {}

void Agent::updateProfitLoss(int profitOrLoss) {
    if (profitOrLoss > biggestGain_) {
        biggestGain_ = profitOrLoss;
    }
    if (profitOrLoss < biggestLoss_) {
        biggestLoss_ = profitOrLoss;
    }
}

void Agent::buyStock(int stockId, int quantity, double pricePerShare) {
    double totalPurchaseCost = quantity * pricePerShare;

    if (!ownsStock(stockId)) {
        OwnedStock info;
        info.quantity  = quantity;
        info.costBasis = pricePerShare;
        owned_stocks_[stockId] = info;
    } else {
        OwnedStock &info = owned_stocks_[stockId];
        double oldTotalCost = info.costBasis * info.quantity;
        double newTotalCost = oldTotalCost + totalPurchaseCost;
        int    newQuantity  = info.quantity + quantity;

        info.quantity  = newQuantity;
        info.costBasis = newTotalCost / static_cast<double>(newQuantity);
    }
}