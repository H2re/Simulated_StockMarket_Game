#include "stock.h"


double Stock::sell(unsigned int shares, double price) {
    if (price < 0 || shares <= 0 || shares > availableShares_) return 0.0;

    // Place sell order and adjust total shares
    order_book_.placeSellOrder(shares, price);

    double newPrice = (price_ * (totShares_ - availableShares_) + (price * shares)) / (totShares_ - availableShares_ + shares);
    availableShares_ += shares;

    updateStock(newPrice);
    return price * shares;
}

unsigned int Stock::getAvailableShares() const {
    return availableShares_;
}

double Stock::getPrice() const {
    return price_;
}

double Stock::getGrowth() const {
    return growth_;
}

std::string Stock::getName() const {
    return name_;
}

void Stock::buy(unsigned int shares, double price) {
    if (price < 0 || shares <= 0) return;
    order_book_.placeBuyOrder(shares, price);
    double newPrice = (price_ * (totShares_ - availableShares_) + (price * shares)) / (totShares_ - availableShares_ + shares);
    availableShares_ -= shares;
    updateStock(newPrice);
}

void Stock::updateStock(double value) {
    history_.enqueue(value);
    double value1, value2, change;
    double sumPrice = 0.0, sumChanges = 0.0;
    if (history_.size() >= 2) {
        value1 = history_[history_.size() - 1];
        value2 = history_[history_.size() - 2];
        change = ((value1 - value2) / value2) * 100;
        changes_.enqueue(change);
    }

    for (unsigned int i = 0; i < history_.size(); i++) {
        sumPrice += history_[i];
        if (i == 0) continue;
        sumChanges += changes_[i];
    }
    // Update price and growth
    price_ = value;
    growth_ = (changes_.size() > 0) ? sumChanges / changes_.size() : 0.0;
}

std::ostream& operator<<(std::ostream& ostr, const Stock& stock) {
    ostr << "Stock: " << stock.getName() << "\n";
    ostr << "Price: " << stock.getPrice() << "\n";
    ostr << "Growth: " << stock.getGrowth() << "%\n";
    ostr << "Available Shares: " << stock.getAvailableShares() << "\n";
    ostr << "History: ";
    const Queue& history = stock.getHistory();
    for (unsigned int i = 0; i < history.size(); ++i) {
        ostr << history[i] << " ";
    }
    ostr << std::endl;
    ostr << "Change: ";
    const Queue& change = stock.getChange();
    for (unsigned int i = 0; i < change.size(); ++i) {
        ostr << change[i] << " ";
    }
    ostr << std::endl;
    return ostr;
}
