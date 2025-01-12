#ifndef STOCK_MARKET_H
#define STOCK_MARKET_H

#include <unordered_map>
#include <string>

#include "stock.h"
#include "transaction_logger.h"

class StockMarket {
public:
    StockMarket(TransactionLogger &logger) : logger_(logger) {};

    bool addNewStock(int id, const std::string &name, int value, int volatility,
                        double dividendYield, const std::string &sector);
                        
    std::unordered_map<int, Stock> getStocksSnapshot();
    int getStockValue(int stockId);
    double calculateTrend(const Stock &stock);
    double getDividendYield(int stockId);

    void adjustAllStocks(int currentRound);
    void recordTransaction(int stockId, int currentRound);
    void applyInstantPriceChange(int stockId, int change);
    void applySectorEvent(const std::string &sector, int impact);
    void applyMacroEvent();
    void applySectorRotation();
private:
    std::mutex mutex_;
    TransactionLogger &logger_;
    std::unordered_map<int, Stock> stocks_;
};

#endif // STOCK_MARKET_H