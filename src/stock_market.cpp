#include "../include/constants.h"
#include "../include/utils.h"
#include "../include/stock_market.h"

bool StockMarket::addNewStock(int id, const std::string &name, int value, 
                              int volatility, double dividendYield, 
                              const std::string &sector)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (stocks_.count(id) == 0) {
        stocks_[id] = Stock(id, name, value, volatility, dividendYield, sector);
        logger_.log("New stock added: " 
                    + std::to_string(id) + " (" + name + "), "
                    + "Value=" + std::to_string(value) 
                    + ", Volatility=" + std::to_string(volatility)
                    + ", Dividend=" + to_string_with_precision(dividendYield, 2)
                    + ", Sector=" + sector);
        return true;
    }
    return false;
}

void StockMarket::adjustAllStocks(int currentRound) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (std::unordered_map<int, Stock>::iterator it = stocks_.begin(); 
         it != stocks_.end(); )
    {
        it->second.adjustValue(logger_, currentRound);

        // Chance of an earnings announcement
        if (generate_random(0, 100) < 5) {
            it->second.applyEarningsAnnouncement(logger_);
        }

        // Remove stock if value is 0
        if (it->second.getValue() == 0) {
            logger_.log("Stock " + std::to_string(it->first) + " ("
                        + it->second.getName() + ") removed from the market (value=0).");
            it = stocks_.erase(it);
        } else {
            ++it;
        }
    }
}

void StockMarket::recordTransaction(int stockId, int currentRound) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stocks_.count(stockId)) {
        Stock &stock = stocks_[stockId];
        stock.incrementTransactionCount();
        stock.setLastTradedRound(currentRound);
    }
}

void StockMarket::applyInstantPriceChange(int stockId, int change) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stocks_.count(stockId)) {
        stocks_[stockId].applyInstantPriceChange(change);
    }
}

void StockMarket::applySectorEvent(const std::string &sector, int impact) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (std::pair<const int, Stock> &p : stocks_) {
        Stock &stock = p.second;
        if (stock.getSector() == sector) {
            stock.applySectorImpact(impact);
            logger_.log("Sector Event: " + sector + " impacted Stock " +
                        std::to_string(stock.getId()) + " (" + stock.getName() + ") by " +
                        std::to_string(impact));
        }
    }
}

void StockMarket::applyMacroEvent() {
    std::lock_guard<std::mutex> lock(mutex_);
    double percentage = generate_random_real(MACRO_EVENT_MIN_IMPACT, MACRO_EVENT_MAX_IMPACT);

    for (std::pair<const int, Stock> &p : stocks_) {
        Stock &stock = p.second;
        int oldValue = stock.getValue();
        int change   = static_cast<int>(oldValue * percentage);
        stock.applyInstantPriceChange(change);
    }

    logger_.log("[MacroEvent] Global market shifted by " +
                to_string_with_precision(percentage * 100, 2) + "%");
}

void StockMarket::applySectorRotation() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (SECTORS.size() < 2) return;

    int idx1 = generate_random(0, (int)SECTORS.size() - 1);
    int idx2 = generate_random(0, (int)SECTORS.size() - 1);
    while (idx2 == idx1) {
        idx2 = generate_random(0, (int)SECTORS.size() - 1);
    }
    std::string boomSector = SECTORS[idx1];
    std::string bustSector = SECTORS[idx2];

    int boomImpact = generate_random(10, 30);
    int bustImpact = -generate_random(10, 30);

    logger_.log("[SectorRotation] " + boomSector + " booms (" 
                + std::to_string(boomImpact) + "), "
                + bustSector + " busts (" + std::to_string(bustImpact) + ").");

    for (std::pair<const int, Stock> &p : stocks_) {
        Stock &stock = p.second;
        if (stock.getSector() == boomSector) {
            stock.applySectorImpact(boomImpact);
        } else if (stock.getSector() == bustSector) {
            stock.applySectorImpact(bustImpact);
        }
    }
}

int StockMarket::getStockValue(int stockId) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stocks_.count(stockId)) {
        return stocks_[stockId].getValue();
    }
    return 0;
}

std::unordered_map<int, Stock> StockMarket::getStocksSnapshot() {
    std::lock_guard<std::mutex> lock(mutex_);
    return stocks_;
}

double StockMarket::calculateTrend(const Stock &stock) {
    const std::deque<int> &history = stock.getHistory();
    if (history.size() < 2) {
        return 0.0;
    }

    double total_change = 0.0;
    for (size_t i = 1; i < history.size(); ++i) {
        total_change += (history[i] - history[i - 1]);
    }
    return total_change / (history.size() - 1);
}

double StockMarket::getDividendYield(int stockId) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stocks_.count(stockId)) {
        return stocks_[stockId].getDividendYield();
    }
    return 0.0;
}