#ifndef __STOCK_H__
#define __STOCK_H__

#include <string>
#include <deque>

#include "transaction_logger.h"

class Stock {
public:
    Stock();
    Stock(int id, const std::string &name, int value, int volatility,
          double dividendYield, const std::string &sector);

    /* GETTERS */
    const std::string &getName() const { return name_; }
    int getId() const { return id_; }
    int getValue() const { return value_; }
    int getVolatility() const { return volatility_; }
    int getTransactionCount() const { return transaction_count_; }
    int getLastTradedRound() const { return last_traded_round_; }
    double getDividendYield() const { return dividend_yield_; }
    const std::string &getSector() const { return sector_; }
    const std::deque<int> &getHistory() const { return history_; }

    /* ACCESSORS */
    void incrementTransactionCount() { transaction_count_++; }
    void setLastTradedRound(int round) { last_traded_round_ = round; }
    void applyInstantPriceChange(int change);
    void applySectorImpact(int impact);
    void applyEarningsAnnouncement(TransactionLogger &logger);
    void adjustValue(TransactionLogger &logger, int currentRound);

private:
    int id_;
    std::string name_;
    int value_;
    int volatility_;
    int transaction_count_;
    int last_traded_round_;
    std::deque<int> history_;
    double dividend_yield_;
    std::string sector_;
};

#endif /* __STOCK_H__ */