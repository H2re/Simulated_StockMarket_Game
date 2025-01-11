#ifndef __MARKET_H__
#define __MARKET_H__

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <ostream>

#include "../stock/stock.h"

class Market {
    public:
        Market();
        Market(const std::string& stockFile);
        Stock& operator[](std::string stock) { return stocks_[stock]; }
        friend std::ostream& operator<<(std::ostream& os, const Market& market);
        
    private:
        std::map<std::string, Stock> stocks_;
};


std::ostream& operator<<(std::ostream& ostr, const Market& market) {
    unsigned int cnt = 0;
    for(std::map<std::string, Stock>::const_iterator itr = market.stocks_.begin(); itr != market.stocks_.end(); itr++) {
        cnt++;
        ostr << cnt << " " << itr->second << std::endl;
    }
    return ostr;
}

Market::Market(const std::string& stockFile) {
    std::ifstream infile(stockFile);
    std::string stockName;
    unsigned int marketCap;
    while(infile >> stockName >> marketCap) {
        Stock stock(stockName, marketCap);
        stocks_[stockName] = stock;
    }


}
#endif // __MARKET_H__

