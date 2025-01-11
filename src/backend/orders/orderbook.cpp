#include "orderbook.h"

void Orderbook::placeBuyOrder(unsigned int quantity, double price) {
    // Create a new order
    Order order = {nextOrderId_++, price, quantity};

    // Place the order in the buy orders map
    buy_orders_[price] = order;
    std::cout << "Placed Buy Order - ID: " << order.id << ", Price: " << price << ", Quantity: " << quantity << "\n";
}

void Orderbook::placeSellOrder(unsigned int quantity, double price) {
    Order order = {nextOrderId_++, price, quantity};

    // Place the order in the sell orders map
    sell_orders_[price] = order;
    std::cout << "Placed Sell Order - ID: " << order.id << ", Price: " << price << ", Quantity: " << quantity << "\n";
}

void Orderbook::matchOrders() {
    // Iterate through the buy orders (highest price first) and sell orders (lowest price first)
    for (auto buyIt = buy_orders_.rbegin(); buyIt != buy_orders_.rend();) {
        for (auto sellIt = sell_orders_.begin(); sellIt != sell_orders_.end();) {
            // Match only if buy price >= sell price
            if (buyIt->first >= sellIt->first) {
                unsigned int matchedQuantity = std::min(buyIt->second.quantity, sellIt->second.quantity);

                std::cout << "Matched Order - Buy ID: " << buyIt->second.id << " with Sell ID: " << sellIt->second.id
                          << ", Quantity: " << matchedQuantity << ", Price: " << buyIt->first << "\n";

                // Update quantities
                buyIt->second.quantity -= matchedQuantity;
                sellIt->second.quantity -= matchedQuantity;

                // Remove orders if they are fully matched
                if (buyIt->second.quantity == 0) {
                    buyIt = std::next(buyIt);  // Move to next buy order
                }

                if (sellIt->second.quantity == 0) {
                    sellIt = std::next(sellIt);  // Move to next sell order
                }
            } else {
                break;  // No further matches possible for this buy order
            }
        }
    }
}

void Orderbook::displayOrderBook() const {
    std::cout << "Buy Orders: \n";
    for (auto it = buy_orders_.rbegin(); it != buy_orders_.rend(); ++it) {
        std::cout << "ID: " << it->second.id << ", Price: " << it->second.price << ", Quantity: " << it->second.quantity << "\n";
    }

    std::cout << "Sell Orders: \n";
    for (auto it = sell_orders_.begin(); it != sell_orders_.end(); ++it) {
        std::cout << "ID: " << it->second.id << ", Price: " << it->second.price << ", Quantity: " << it->second.quantity << "\n";
    }
}
