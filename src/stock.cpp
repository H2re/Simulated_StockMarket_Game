#include "../include/constants.h"
#include "../include/utils.h"
#include "../include/stock.h"

Stock::Stock() : 
    id_(0),
    name_(),
    value_(0),
    volatility_(0),
    transaction_count_(0),
    last_traded_round_(0),
    dividend_yield_(0.0),
    sector_() {}

Stock::Stock(int id, const std::string &name, int value, int volatility,
             double dividendYield, const std::string &sector) : 
    id_(id),
    name_(name),
    value_(value),
    volatility_(volatility),
    transaction_count_(0),
    last_traded_round_(0),
    dividend_yield_(dividendYield),
    sector_(sector) {}

void Stock::adjustValue(TransactionLogger &logger, int /*currentRound*/) {
    int old_value = value_;

    // If transaction_count_ is high, increase the value
    if (transaction_count_ > 5) {
        int increment = generate_random(volatility_ / 2, volatility_);
        value_ += increment;
    }
    // If transaction_count_ == 0, decrease the value
    else if (transaction_count_ == 0) {
        int decrement = generate_random(volatility_ / 4, volatility_ / 2);
        value_ -= decrement;
    }

    // Random small market fluctuation
    if (generate_random(0, 100) < 20) {
        value_ += generate_random(-10, 10);
    }

    // Bound checks
    if (value_ < MIN_STOCK_VALUE) {
        value_ = 0;
    } else if (value_ > MAX_STOCK_VALUE) {
        value_ = MAX_STOCK_VALUE;
    }

    // Track history
    history_.push_back(value_);
    if (history_.size() > 20) {
        history_.pop_front();
    }

    // Reset transaction count for the next round
    transaction_count_ = 0;
}

void Stock::applyInstantPriceChange(int change) {
    value_ += change;
    if (value_ < MIN_STOCK_VALUE) {
        value_ = MIN_STOCK_VALUE;
    } else if (value_ > MAX_STOCK_VALUE) {
        value_ = MAX_STOCK_VALUE;
    }
}

void Stock::applySectorImpact(int impact) {
    value_ += impact;
    if (value_ < MIN_STOCK_VALUE) {
        value_ = 0;
    } else if (value_ > MAX_STOCK_VALUE) {
        value_ = MAX_STOCK_VALUE;
    }
}

void Stock::applyEarningsAnnouncement(TransactionLogger &logger) {
    // 50% chance of a positive surprise, 50% negative
    int surpriseDirection = (generate_random(0, 1) == 0) ? 1 : -1;
    // Scale of surprise: up to +/- (volatility * 2)
    int magnitude = generate_random(volatility_, volatility_ * 2);
    int change = surpriseDirection * magnitude;
    applyInstantPriceChange(change);

    logger.log("[Earnings] Stock " + std::to_string(id_) + " (" + name_ + 
               ") had an earnings " + 
               (surpriseDirection > 0 ? "beat" : "miss") +
               " with an impact of " + std::to_string(change));
}