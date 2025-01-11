#include "backend/market/market.h"

#define dist std::uniform_real_distribution<>
int main() {
    std::random_device rd;
    const int num_agents = 20;
    Market market("stocks.txt");
    market["AAPL"].buy(100, 150);
    market["AAPL"].sell(50, 160);
    market["AAPL"].buy(200, 155);
    market["AAPL"].sell(50, 160);
    market["AAPL"].buy(200, 155);
    market["AAPL"].sell(50, 160);
    market["AAPL"].buy(200, 155);
    std::cout << market["AAPL"];
    std::cout<< "What Stocks would you like to purchase? "; 

    return 0;
}
