#ifndef __ORDERBOOK_H__
#define __ORDERBOOK_H__

#include <iostream>
#include <string>
#include <map>

class Orderbook {
public:
    struct Order {
        unsigned int id;
        double price;
        unsigned int quantity;
        unsigned long timestamp;
        bool operator<(const Order& other) const { return price < other.price; }
    };

    Orderbook() : stock_(""), nextOrderId_(0) {};
    Orderbook(const std::string& stock) : stock_(stock), nextOrderId_(1) {};
    void placeBuyOrder(unsigned int quantity, double price);
    void placeSellOrder(unsigned int quantity, double price);
    void matchOrders();
    void displayOrderBook() const;
private:
    struct SellOrderComparator {
        bool operator()(const Order& a, const Order& b) const { return a.price > b.price; }
    };
    std::map<double, Order> buy_orders_;
    std::map<double, Order> sell_orders_;
    std::string stock_;
    unsigned int nextOrderId_;
};

#endif // __ORDERBOOK_H__